#ifndef DIALOGS_HPP
#define DIALOGS_HPP

#include <string>
#include <SFGUI/Widget.hpp>
#include <SFGUI/Desktop.hpp>

/**
 * Displays a box with Yes and No buttons, along with a prompt
 */
bool yesnobox(sfg::Desktop& dk, sfg::Widget::Ptr parent,
              const std::string& title, const std::string& prompt);

/**
 * Displays a message and an Ok button
 */
void showmessage(sfg::Desktop& dk, sfg::Widget::Ptr parent,
                 const std::string& title, const std::string& message);

#endif // DIALOGS_HPP
