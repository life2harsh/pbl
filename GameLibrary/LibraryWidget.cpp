#include "LibraryWidget.h"
#include "DataStore.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QProcess>
#include <QDebug>

LibraryWidget::LibraryWidget(QWidget* parent)
    : QWidget(parent)
    , view(new QTableView(this))
    , model(new QStandardItemModel(this))
{
    model->setColumnCount(5);
    model->setHorizontalHeaderLabels(
        { "Title", "Platform", "Hours", "Last Played", "ExecPath" }
        );

    view->setModel(model);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->horizontalHeader()->setStretchLastSection(true);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setColumnHidden(4, true);  // hide ExecPath

    auto* logoutBtn = new QPushButton("Logout", this);
    auto* layout    = new QVBoxLayout(this);
    layout->addWidget(view);
    layout->addWidget(logoutBtn);
    setLayout(layout);

    connect(logoutBtn, &QPushButton::clicked,
            this,       &LibraryWidget::logoutRequested);

    connect(view, &QTableView::doubleClicked, this,
            [=](const QModelIndex& idx) {
                int row = idx.row();
                QString execPath = model->item(row, 4)->text();
                if (!execPath.isEmpty()) {
                    if (!QProcess::startDetached(execPath)) {
                        qDebug() << "[LibraryWidget] Failed to launch" << execPath;
                    }
                }
            }
            );
}

void LibraryWidget::load(const QString& username) {
    qDebug() << "[LibraryWidget] load() called for user:" << username;

    model->removeRows(0, model->rowCount());

    // ✅ HARD-CODED ABSOLUTE PATH (USE DOUBLE BACKSLASHES OR FORWARD SLASHES)
    QString gamesFile = "H:/gamelib/games.json";
    qDebug() << "[LibraryWidget] Reading JSON from:" << gamesFile;

    auto games = DataStore::loadGames(username, gamesFile);
    qDebug() << "[LibraryWidget] DataStore returned" << games.size() << "games";

    for (const auto& g : games) {
        QList<QStandardItem*> row;
        row << new QStandardItem(g.title)
            << new QStandardItem(g.platform)
            << new QStandardItem(QString::number(g.hoursPlayed))
            << new QStandardItem(g.lastPlayed)
            << new QStandardItem(g.execPath);
        model->appendRow(row);
    }
}
