// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

//! [0]
class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    Q_DISABLE_COPY_MOVE(TreeModel)

    explicit TreeModel(const QString &data, QObject *parent = nullptr);
    ~TreeModel() override;

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(
        int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /**
     * @brief 获取目标位置项的 QModelIndex
     * @param row 行号
     * @param column 列号
     * @param parent 父节点的  QModelIndex
     * @return
     */
    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;

    /**
     * @brief 获取当前节点父节点的 QModelIndex
     * @param index 当前节点
     * @return
     */
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;

private:
    /**
     * @brief 根据文档中每行的缩进生成底层数据的树状索引
     * @param lines 文档中读入的原始数据（连续）
     * @param parent 首行 header 头结点指针
     */
    static void setupModelData(const QList<QStringView> &lines, TreeItem *parent);

    std::unique_ptr<TreeItem> rootItem;
};
//! [0]

#endif // TREEMODEL_H
