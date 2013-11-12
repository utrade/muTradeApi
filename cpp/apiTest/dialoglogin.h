#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include <QDialog>
#include "context.h"

namespace Ui {

class DialogLogin;
}

/**
 * @brief The DialogLogin class
 */
class DialogLogin : public QDialog
{
    Q_OBJECT
    
public:
    /**
     * @brief DialogLogin
     * @param parent
     */
    explicit DialogLogin(QWidget *parent = 0);
    ~DialogLogin();
    /**
     * @brief getUser
     * @return
     */
    mutrade::Int32 getUser(){return uid;}
    /**
     * @brief getPswd
     * @return
     */
    mutrade::String getPswd(){return pswd;}

    /**
     * @brief clearForm
     */
    void clearForm();
    /**
     * @brief reqLogin
     * @param user
     * @param pswd
     * @param restoreState
     */
    void reqLogin(mutrade::Int32 user, const std::string &pswd, bool restoreState);
    /**
     * @brief reqLogout
     */
    void reqLogout();
private slots:
    /**
     * @brief on_pushButtonLogin_clicked
     */
    void on_pushButtonLogin_clicked();

private:
    /**
     * @brief ui
     */
    Ui::DialogLogin *ui;
    /**
     * @brief uid
     */
    mutrade::Int32 uid;
    /**
     * @brief pswd
     */
    mutrade::String pswd;
};

#endif // DIALOGLOGIN_H
