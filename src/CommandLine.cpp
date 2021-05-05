/*****************************************************************************
 *
 *  PROJECT:            RakCloudDesktop
 *  LICENSE:            See LICENSE in the top level directory
 *  FILE:               CommandLine.cpp
 *  DESCRIPTION:        Command line widget
 *  COPYRIGHT:          (c) 2021 RINWARES <rinwares.com>
 *  AUTHOR:             Rinat Namazov <rinat.namazov@rinwares.com>
 *
 *****************************************************************************/

#include "CommandLine.hpp"

#include <QKeyEvent>

CommandLine::CommandLine(QWidget* parent, int historyMaxSize)
    : QLineEdit(parent), m_historyCurPos(-1) {
    setHistoryMaxSize(historyMaxSize);
}

void CommandLine::setHistoryMaxSize(int historyMaxSize) {
    if (historyMaxSize < m_history.size()) {
        if (historyMaxSize > 0) {
            m_history = m_history.mid(0, historyMaxSize);
        } else {
            m_history.clear();
        }
    } else {
        m_history.reserve(historyMaxSize);
    }
    m_historyMaxSize = historyMaxSize;
}

void CommandLine::resetCurrentHistoryPos() { setCurrentHistoryPos(-1); }

void CommandLine::setCurrentHistoryPos(int historyPos) {
    if (historyPos >= -1 && historyPos < m_history.size()) {
        m_historyCurPos = historyPos;
    }
}

void CommandLine::addToHistory() { pushHistory(text().trimmed()); }

void CommandLine::addToHistoryAndResetPos() {
    pushHistory(text().trimmed());
    resetCurrentHistoryPos();
    clear();
}

void CommandLine::clearHistory() {
    m_history.clear();
    m_historyCurPos = -1;
}

void CommandLine::pushHistory(QString historyText) {
    if (m_historyMaxSize <= 0) {
        return;
    }
    if (!historyText.isEmpty()) {
        if (!m_history.isEmpty()) {
            if (historyText == m_history.first()) {
                return;
            }
        }
        if (m_history.size() >= m_historyMaxSize) {
            m_history.removeLast();
        }
        m_history.prepend(historyText);
    }
}

void CommandLine::popHistory() {
    if (!m_history.isEmpty()) {
        if (m_historyCurPos == (m_history.size() - 1)) {
            --m_historyCurPos;
            if (m_historyCurPos == -1) {
                setText(m_lastText);
            } else {
                setText(m_history.at(m_historyCurPos));
            }
        }
        m_history.removeLast();
    }
}

void CommandLine::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Up: {
            if (!m_history.isEmpty()) {
                if (m_historyCurPos < m_history.count()) {
                    if (m_historyCurPos == -1) {
                        m_lastText = text();
                    }
                    if ((m_historyCurPos + 1) < m_history.count()) {
                        setText(m_history.at(++m_historyCurPos));
                    }
                }
            }
            break;
        }
        case Qt::Key_Down: {
            if (m_historyCurPos > -1 && m_historyCurPos < m_history.size()) {
                --m_historyCurPos;
                if (m_historyCurPos == -1) {
                    setText(m_lastText);
                } else {
                    setText(m_history.at(m_historyCurPos));
                }
            }
            break;
        }
    }
    QLineEdit::keyPressEvent(event);
}
