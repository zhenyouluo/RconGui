/**
 * \file
 *
 * \author Mattia Basaglia
 *
 * \section License
 *
 * Copyright (C) 2015 Mattia Basaglia
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef SERVER_SETUP_WIDGET_HPP
#define SERVER_SETUP_WIDGET_HPP

#include "ui_server_setup_widget.h"
#include "xonotic/connection_details.hpp"

/**
 * \brief Form used to gather Xonotic connection information
 */
class ServerSetupWidget : public QWidget, private Ui::ServerSetupWidget
{
    Q_OBJECT

public:
    ServerSetupWidget(QWidget* parent = nullptr);

    ServerSetupWidget(const xonotic::ConnectionDetails& xonotic, QWidget* parent = nullptr);

    /**
     * \brief Builds a Xonotic connection from the form contents
     */
    xonotic::ConnectionDetails connection_details() const;

    /**
     * \brief Populates the form input fields from the server settings
     */
    void populate(const xonotic::ConnectionDetails& xonotic);

private slots:
    void on_button_save_clicked();
    void on_button_delete_clicked();
    void on_input_preset_currentIndexChanged(const QString& text);
    void update_placeholder();

private:
    /**
     * \brief Loads the saved servers in the preset combo box
     */
    void update_presets();
};

#endif // SERVER_SETUP_WIDGET_HPP
