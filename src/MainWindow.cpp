#include "MainWindow.hpp"
#include <gtkmm/grid.h>
#include <gtkmm/menuitem.h>
#include <gtkmm/aboutdialog.h>


MainWindow::MainWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder)
    : Gtk::Window{cobject}
    , m_fullscreen{false}
{
    set_default_size(1280, 720);

    auto const appIcon16x16   = Gdk::Pixbuf::create_from_resource("/main/icon/16x16.ico");
    auto const appIcon32x32   = Gdk::Pixbuf::create_from_resource("/main/icon/32x32.ico");
    auto const appIcon48x48   = Gdk::Pixbuf::create_from_resource("/main/icon/48x48.ico");
    auto const appIcon64x64   = Gdk::Pixbuf::create_from_resource("/main/icon/64x64.ico");
    auto const appIcon128x128 = Gdk::Pixbuf::create_from_resource("/main/icon/128x128.ico");
    auto const appIcon256x256 = Gdk::Pixbuf::create_from_resource("/main/icon/256x256.ico");
    set_icon_list({appIcon16x16, appIcon32x32, appIcon48x48, appIcon64x64, appIcon128x128, appIcon256x256});

    Gtk::Grid* mainGrid = nullptr;
    refBuilder->get_widget("main_grid", mainGrid);
    mainGrid->attach(m_webView, 0, 1, 1, 1);
    m_webView.set_vexpand();

    Gtk::MenuItem* refreshMenuItem = nullptr;
    refBuilder->get_widget("refresh_menu_item", refreshMenuItem);
    refreshMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onRefresh));

    Gtk::MenuItem* quitMenuItem = nullptr;
    refBuilder->get_widget("quit_menu_item", quitMenuItem);
    quitMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onQuit));

    Gtk::MenuItem* fullscreenMenuItem = nullptr;
    refBuilder->get_widget("fullscreen_menu_item", fullscreenMenuItem);
    fullscreenMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onFullscreen));

    Gtk::MenuItem* aboutMenuItem = nullptr;
    refBuilder->get_widget("about_menu_item", aboutMenuItem);
    aboutMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onAbout));

    signal_window_state_event().connect(sigc::mem_fun(this, &MainWindow::onWindowStateEvent));

    show_all();
}

bool MainWindow::onWindowStateEvent(GdkEventWindowState* event)
{
    m_fullscreen = event->new_window_state & GDK_WINDOW_STATE_FULLSCREEN;
    return false;
}

void MainWindow::onRefresh()
{
    m_webView.refresh();
}

void MainWindow::onQuit()
{
    close();
}

void MainWindow::onFullscreen()
{
    m_fullscreen ? unfullscreen() : fullscreen();
}

void MainWindow::onAbout()
{
    auto aboutDialog = Gtk::AboutDialog{};

    aboutDialog.set_title("About");
    aboutDialog.set_program_name("whatsapp-for-linux");
    aboutDialog.set_logo_icon_name("help-about");
    aboutDialog.set_comments("An unofficial WhatsApp linux client desktop application.");
    aboutDialog.set_website("https://github.com/eneshecan/whatsapp-for-linux");
    aboutDialog.set_website_label("Github Repo");
    aboutDialog.set_license_type(Gtk::LICENSE_GPL_3_0);

    aboutDialog.run();
}
