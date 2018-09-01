/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Exit;
    QAction *action_F5;
    QAction *action;
    QAction *action_2;
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab_sys;
    QTabWidget *tabWidget_2;
    QWidget *tabWidget_2Page1;
    QWidget *tabWidget_2Page2;
    QWidget *tabWidget_2Page3;
    QWidget *tab_process;
    QLabel *label_2;
    QWidget *tab_module;
    QLabel *label_3;
    QWidget *tab_user;
    QWidget *tab_mistake;
    QMenuBar *menuBar;
    QMenu *menu_F;
    QMenu *menu_E;
    QMenu *menu_H;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(927, 614);
        action_Exit = new QAction(MainWindow);
        action_Exit->setObjectName(QStringLiteral("action_Exit"));
        action_F5 = new QAction(MainWindow);
        action_F5->setObjectName(QStringLiteral("action_F5"));
        action = new QAction(MainWindow);
        action->setObjectName(QStringLiteral("action"));
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QStringLiteral("action_2"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(4, 9, 901, 541));
        tab_sys = new QWidget();
        tab_sys->setObjectName(QStringLiteral("tab_sys"));
        tabWidget_2 = new QTabWidget(tab_sys);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(10, 10, 881, 491));
        tabWidget_2->setContextMenuPolicy(Qt::DefaultContextMenu);
        tabWidget_2->setTabPosition(QTabWidget::West);
        tabWidget_2->setTabShape(QTabWidget::Triangular);
        tabWidget_2->setElideMode(Qt::ElideLeft);
        tabWidget_2->setUsesScrollButtons(true);
        tabWidget_2->setDocumentMode(false);
        tabWidget_2->setTabsClosable(false);
        tabWidget_2->setMovable(false);
        tabWidget_2->setTabBarAutoHide(false);
        tabWidget_2Page1 = new QWidget();
        tabWidget_2Page1->setObjectName(QStringLiteral("tabWidget_2Page1"));
        tabWidget_2->addTab(tabWidget_2Page1, QString());
        tabWidget_2Page2 = new QWidget();
        tabWidget_2Page2->setObjectName(QStringLiteral("tabWidget_2Page2"));
        tabWidget_2->addTab(tabWidget_2Page2, QString());
        tabWidget_2Page3 = new QWidget();
        tabWidget_2Page3->setObjectName(QStringLiteral("tabWidget_2Page3"));
        tabWidget_2->addTab(tabWidget_2Page3, QString());
        tabWidget->addTab(tab_sys, QString());
        tab_process = new QWidget();
        tab_process->setObjectName(QStringLiteral("tab_process"));
        label_2 = new QLabel(tab_process);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(250, 220, 80, 20));
        tabWidget->addTab(tab_process, QString());
        tab_module = new QWidget();
        tab_module->setObjectName(QStringLiteral("tab_module"));
        label_3 = new QLabel(tab_module);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(290, 310, 80, 20));
        tabWidget->addTab(tab_module, QString());
        tab_user = new QWidget();
        tab_user->setObjectName(QStringLiteral("tab_user"));
        tabWidget->addTab(tab_user, QString());
        tab_mistake = new QWidget();
        tab_mistake->setObjectName(QStringLiteral("tab_mistake"));
        tabWidget->addTab(tab_mistake, QString());
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 927, 32));
        menu_F = new QMenu(menuBar);
        menu_F->setObjectName(QStringLiteral("menu_F"));
        menu_E = new QMenu(menuBar);
        menu_E->setObjectName(QStringLiteral("menu_E"));
        menu_H = new QMenu(menuBar);
        menu_H->setObjectName(QStringLiteral("menu_H"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu_F->menuAction());
        menuBar->addAction(menu_E->menuAction());
        menuBar->addAction(menu_H->menuAction());
        menu_F->addAction(action_Exit);
        menu_E->addAction(action_F5);
        menu_H->addAction(action);
        menu_H->addAction(action_2);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        action_Exit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272(W)", nullptr));
#ifndef QT_NO_SHORTCUT
        action_Exit->setShortcut(QApplication::translate("MainWindow", "Alt+W", nullptr));
#endif // QT_NO_SHORTCUT
        action_F5->setText(QApplication::translate("MainWindow", "\345\210\267\346\226\260(F5)", nullptr));
#ifndef QT_NO_SHORTCUT
        action_F5->setShortcut(QApplication::translate("MainWindow", "F5", nullptr));
#endif // QT_NO_SHORTCUT
        action->setText(QApplication::translate("MainWindow", "\344\275\277\347\224\250\350\257\264\346\230\216", nullptr));
        action_2->setText(QApplication::translate("MainWindow", "\345\205\263\344\272\216", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabWidget_2Page1), QApplication::translate("MainWindow", "\347\263\273\347\273\237\347\256\200\345\206\265", nullptr));
        tabWidget_2->setTabToolTip(tabWidget_2->indexOf(tabWidget_2Page1), QApplication::translate("MainWindow", "12", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabWidget_2Page2), QApplication::translate("MainWindow", "CPU", nullptr));
        tabWidget_2->setTabToolTip(tabWidget_2->indexOf(tabWidget_2Page2), QApplication::translate("MainWindow", "12", nullptr));
        tabWidget_2->setTabWhatsThis(tabWidget_2->indexOf(tabWidget_2Page2), QApplication::translate("MainWindow", "34", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabWidget_2Page3), QApplication::translate("MainWindow", "\345\206\205\345\255\230", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_sys), QApplication::translate("MainWindow", "\347\263\273\347\273\237\344\277\241\346\201\257", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "222", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_process), QApplication::translate("MainWindow", "\350\277\233\347\250\213", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "333", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_module), QApplication::translate("MainWindow", "\346\250\241\345\235\227\344\277\241\346\201\257", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_user), QApplication::translate("MainWindow", " \347\224\250\346\210\267", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_mistake), QApplication::translate("MainWindow", "\351\224\231\350\257\257\346\212\245\345\221\212", nullptr));
        menu_F->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266(F)", nullptr));
        menu_E->setTitle(QApplication::translate("MainWindow", "\347\274\226\350\276\221(E)", nullptr));
        menu_H->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251(H)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
