// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

/*
    treemodel.cpp

    Provides a simple tree model to show how to create and use hierarchical
    models.
*/

#include "treemodel.h"
#include "treeitem.h"

#include <QStringList>

using namespace Qt::StringLiterals;


TreeModel::TreeModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
    , rootItem(std::make_unique<TreeItem>(QVariantList{tr("Title"), tr("Summary")}))
{
    setupModelData(QStringView{data}.split(u'\n'), rootItem.get());
}

TreeModel::~TreeModel() = default;

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem *>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}




QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};

    const auto *item = static_cast<const TreeItem *>(index.internalPointer());// 取回 QModelIndex 对应的底层数据指针
    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    return index.isValid() ? QAbstractItemModel::flags(index) : Qt::ItemFlags(Qt::NoItemFlags);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return orientation == Qt::Horizontal && role == Qt::DisplayRole ? rootItem->data(section)
                                                                    : QVariant{};
}


QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return {};

    TreeItem *parentItem = parent.isValid() ? static_cast<TreeItem *>(parent.internalPointer())
                                            : rootItem.get();

    // 将底层数据的指针存入 QModelIndex 中，后续可以在 data() 中通过 QModelIndex 取回
    if (auto *childItem = parentItem->child(row))
        return createIndex(row, column, childItem); // QModelIndex 构造器
    return {};
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return {};

    auto *childItem = static_cast<TreeItem *>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();
    // parent 非不可视根节点，生成一个新 QModelIndex; parent 为不可视根节点，返回一个 inValid QModelIndex;
    return parentItem != rootItem.get() ? createIndex(parentItem->row(), 0, parentItem)
                                        : QModelIndex{};
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    const TreeItem *parentItem = parent.isValid()
                                     ? static_cast<const TreeItem *>(parent.internalPointer())
                                     : rootItem.get();

    return parentItem->childCount();
}


void TreeModel::setupModelData(const QList<QStringView> &lines, TreeItem *parent)
{
    struct ParentIndentation
    {
        TreeItem *parent;
        qsizetype indentation;
    };
    // list 中的最后一项作为当前可插入的父项
    QList<ParentIndentation> state{{parent, 0}};
    // 一行一项，根据开头缩进的空格数决定层级
    for (const auto &line : lines) {
        // position 层级
        qsizetype position = 0;
        for (; position < line.length() && line.at(position).isSpace(); ++position) {
        }
        // 获取项数据
        const QStringView lineData = line.sliced(position).trimmed();
        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            const auto columnStrings = lineData.split(u'\t', Qt::SkipEmptyParts);
            QVariantList columnData;
            columnData.reserve(columnStrings.count());
            for (const auto &columnString : columnStrings)
                columnData << columnString.toString();

            if (position > state.constLast().indentation) { // 比 tree 中的上一项的层级低
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.
                auto *lastParent = state.constLast().parent;
                if (lastParent->childCount() > 0)
                    state.append({lastParent->child(lastParent->childCount() - 1), position});
            } else {
                // 找到可以插入的父项（最后一项）
                while (position < state.constLast().indentation && !state.isEmpty())
                    state.removeLast();
            }

            // Append a new item to the current parent's list of children.
            auto *lastParent = state.constLast().parent;
            lastParent->appendChild(std::make_unique<TreeItem>(columnData, lastParent));
        }
    }
}
