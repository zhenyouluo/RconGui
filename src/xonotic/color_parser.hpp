/**
 * \file
 *
 * \author Mattia Basaglia
 *
 * \section License
 *
 * Copyright (C)  Mattia Basaglia
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
#ifndef XONOTIC_COLOR_PARSER_HPP
#define XONOTIC_COLOR_PARSER_HPP

#include <QString>
#include <QColor>
#include <QTextCursor>
#include "regex.hpp"

namespace xonotic {

/**
 * \brief Parses a xonotic-colored string and provide hooks to generate output
 */
class AbstractColorParser
{
public:
    AbstractColorParser() = default;

    explicit AbstractColorParser(QColor default_color,///< Default text color
                         int    min_brightness=0,  ///< Minimum color brightness
                         int    max_brightness=255)///< Maximum color brightness
        : default_color (std::move(default_color)),
          min_brightness(min_brightness),
          max_brightness(max_brightness)
        {}

    /**
     * \brief Turns a string like ^2 or ^x123 into a QColor
     * \return The true color represented by the string, not bounded
     */
    QColor to_color(const QString& color);

    /**
     * \brief Ensures the color brightness is within the accepted range
     */
    QColor bounded_color(const QColor& color);

    /**
     * \brief Converts a qfont index to a string
     */
    static QString qfont_to_string(uint8_t index);

protected:
    /**
     * \brief Parses \c string
     */
    void parse(const QString& string);

    /**
     * \brief Begins a new fragment with the given color
     */
    virtual void on_start(const QColor& color) = 0;
    /**
     * \brief Ends a fragment, should clean up what \c on_start() does
     */
    virtual void on_end() = 0;

    /**
     * \brief Appends a string with the current color
     */
    virtual void on_append_string(const QString& string) = 0;

    /**
     * \brief Adds a new line to the output
     *
     * Note that \c on_end() is called before this and \c on_start()
     * is called afterwards
     */
    virtual void on_new_line() = 0;

    /**
     * \brief Changes the color for subsequent calls to \c on_append_string()
     */
    virtual void on_change_color(const QColor& color) = 0;

    /**
     * \brief Shows a qfont character
     */
    virtual void on_qfont(uint8_t index);

private:
    /**
     * \brief Color from regex match
     */
    QColor match_to_color(const QStringRef& s);

    /**
     * \brief Hex character to integer
     */
    static int hex_to_int(char c)
    {
        if ( c <= '9' && c >= '0' )
            return c - '0';
        if ( c <= 'f' && c >= 'a' )
            return c - 'a' + 0xa;
        if ( c <= 'F' && c >= 'A' )
            return c - 'A' + 0xa;
        return 0;
    }

    /**
     * \brief Pushes the default color
     */
    void push_start();

    /**
     * \brief Cleans up \c push_start (calls push_string())
     */
    void push_end();

    /**
     * \brief Pushes the \c work_string substring [\c start_index, \c end_index) to the output
     */
    void push_string();

    /**
     * \brief Pushes characters to the output
     */
    void push_char(const QCharRef& c);

    /**
     * \brief Pushes a color to \c output (calls push_string())
     */
    void push_color(const QColor& color);

    /**
     * \brief Handles a ^ code
     */
    void handle_caret();

private:
    QString     work_string;    ///< String currently being parsed
    int         start_index=0;  ///< Starting index for a new substring in \c work_string
    int         end_index=0;    ///< Ending index for a new substring in \c work_string
    QColor      default_color=Qt::gray; ///< Default text color
    int         min_brightness=0;       ///< Minimum color brightness
    int         max_brightness=255;     ///< Maximum color brightness

    static regex::Regex regex_xoncolor;      ///< Regex matching color codes
};

/**
 * \brief Parses a xonotic-colored string and formats a QTextDocument accordingly
 */
class ColorParserTextCursor : public AbstractColorParser
{
public:
    using AbstractColorParser::AbstractColorParser;

    /**
     * \brief Turns some text into html
     */
    void convert(const QString& text, QTextCursor *out);

    /**
     * \brief Turns a small fragment into html,
     *        without adding a new line at the end
     */
    void convert_fragment(const QString& text, QTextCursor *out);

    /**
     * \brief Turns some text into html
     */
    void convert(const QStringList& lines, QTextCursor *out);

protected:
    void on_start(const QColor& color) override;
    void on_end() override;
    void on_append_string(const QString& string) override;
    void on_new_line() override;
    void on_change_color(const QColor& color) override;

private:
    /**
     * \brief Sets up the object for parse()
     * \returns Whether it can proceed with the parsing
     */
    bool before_parse(QTextCursor *output);

    /**
     * \brief Cleans up before_parse()
     */
    void after_parse();

    QTextCursor *output=nullptr;

};

/**
 * \brief Parses a xonotic-colored string to a plain string with no formatting
 */
class ColorParserPlainText : public AbstractColorParser
{
public:
    using AbstractColorParser::AbstractColorParser;

    /**
     * \brief Turns some text into html
     */
    QString convert(const QString& text);

    /**
     * \brief Turns a small fragment into html,
     *        without adding a new line at the end
     */
    QString convert_fragment(const QString& text);

    /**
     * \brief Turns some text into html
     */
    QString convert(const QStringList& lines);

protected:
    void on_start(const QColor& color) override;
    void on_end() override;
    void on_append_string(const QString& string) override;
    void on_new_line() override;
    void on_change_color(const QColor& color) override;

private:
    QString output;

};
} // namespace xonotic
#endif // XONOTIC_COLOR_PARSER_HPP
