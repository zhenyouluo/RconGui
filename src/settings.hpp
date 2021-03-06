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
#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QSettings>
#include <QHash>
#include <QColor>
#include <QFont>

#include "xonotic/connection_details.hpp"
#include "xonotic/cvar_expansion.hpp"
#include "model/player_action.hpp"

/**
 * \brief Settings singleton
 */
class Settings
{
public:
    static Settings& instance()
    {
        static Settings singleton;
        return singleton;
    }

    ~Settings();

    /**
     * \brief Loads settings from the permanent storage (called from the ctor)
     */
    void load();

    /**
     * \brief Save settings in the permanent storage (called from the dtor)
     */
    void save();

    /**
     * \brief Get a settings value
     * \tparam T A Type registered to QVariant
     */
    template<class T>
        typename std::remove_reference<T>::type get(const QString& key, T&& default_value)
        {
            return settings.value(key,
                QVariant::fromValue(std::forward<T>(default_value)))
                .template value<typename std::remove_reference<T>::type>();
        }
    /**
     * \brief Get a settings value
     * \tparam T A Type registered to QVariant
     */
    template<class T>
        T get(const QString& key)
        {
            return settings.value(key).value<T>();
        }
    /**
     * \brief Put a settings value
     * \tparam T A Type registered to QVariant
     */
    template<class T>
        void put(const QString& key, T&& value)
        {
            settings.setValue(key, QVariant::fromValue(std::forward<T>(value)));
        }

    /**
     * \brief Get the console history for the given server
     */
    QStringList get_history(const std::string& server) const;
    /**
     * \brief Set the console history for the given server
     */
    void set_history(const std::string& server, QStringList history);

    QHash<QString, xonotic::ConnectionDetails> saved_servers; ///< Server presets
    QHash<QString, QStringList> console_history; ///< Per-server console history

    QColor console_foreground{192, 192, 192};   ///< Default text color for the console
    QColor console_background{Qt::black};       ///< Background color for the console
    int    console_brightness_max{255};         ///< Maximum brightness for console colors
    int    console_brightness_min{80};          ///< Minimum brightness for console colors
    QFont  console_font{"monospace", 10};       ///< Console text font
    int    console_max_history=128;             ///< Number of items in the console history
    CvarExpansion console_expansion = CvarExpansion::ExpandOrWarn;
    QString console_attach_command = "qc_cmd_sv addtolist log_dest_udp $ip";
    QString console_detach_command = "qc_cmd_sv removefromlist log_dest_udp $ip";

    QList<PlayerAction> player_actions = {      ///< Buttons shown in the player table
        {"Kick", "kick # $player.entity",   "im-kick-user"},
        {"Ban",  "kickban #$player.entity", ":/icons/ban-hammer.svg"},
        {"Mute", "mute #$player.entity",    ":/icons/no-chat.svg"},
    };
    CvarExpansion player_actions_expansion = CvarExpansion::NotExpanded;

    std::vector<QPair<QString,QString>> quick_commands = {
        {"Rescan",    "fs_rescan"},
        {"Restart",   "restart"},
        {"End Match", "endmatch"}
    };
    CvarExpansion quick_commands_expansion = CvarExpansion::NotExpanded;


    /// Commands used to request status
    QStringList                 cmd_status = {"status 1", "g_maplist"};
    /// Commands used to request cvars
    QStringList                 cmd_cvarlist = {"cvarlist"};
    /// Command used to change the map
    QString                     cmd_chmap = "chmap $map";

private:
    Settings();

    QSettings settings;

};

/**
 * \brief Shorthand for Settings::instance()
 */
inline Settings& settings() { return Settings::instance(); }

#endif // SETTINGS_HPP
