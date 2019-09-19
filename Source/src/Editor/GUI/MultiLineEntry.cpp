#include "Editor/GUI/MultiLineEntry.hpp"
#include <SFGUI/Context.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
using namespace std;

namespace sfg {

namespace eng {
unique_ptr<RenderQueue> createEntryDrawable(shared_ptr<const MultiLineEntry> entry,
                                            const sf::String& line, int y) {
    Engine& e = Context::Get().GetEngine();
	auto border_color = sf::Color::Black;
	auto background_color = sf::Color::White;
	auto text_color = sf::Color::Black;
	auto cursor_color = sf::Color::Black;
	auto text_padding = e.GetProperty<float>("Padding", entry);
	auto cursor_thickness = e.GetProperty<float>("Thickness", entry);
	auto border_width = e.GetProperty<float>("BorderWidth", entry);
	auto border_color_shift = e.GetProperty<int>("BorderColorShift", entry);
	const auto& font_name = e.GetProperty<string>("FontName", entry);
	const auto& font = e.GetResourceManager().GetFont(font_name);
	auto font_size = e.GetProperty<unsigned int>("FontSize", entry);

	unique_ptr<RenderQueue> queue(new RenderQueue);

	// Pane.
	queue->Add(
		Renderer::Get().CreatePane(
			sf::Vector2f(0.f, 0.f),
			sf::Vector2f(entry->GetAllocation().width, entry->GetAllocation().height),
			border_width,
			background_color,
			border_color,
			-border_color_shift
		)
	);

	auto line_height = e.GetFontLineHeight(*font, font_size);
	sf::Text vis_label(entry->GetText(), *font, font_size);
	vis_label.setFillColor(text_color);
	vis_label.setPosition(text_padding, text_padding);

	queue->Add(Renderer::Get().CreateText(vis_label));

	// Draw cursor if entry is active and cursor is visible.
	if (entry->HasFocus() && entry->IsCursorVisible()) {
		sf::Vector2f metrics(e.GetTextStringMetrics(line, *font, font_size));

		queue->Add(
			Renderer::Get().CreateRect(
				sf::FloatRect(
					metrics.x + text_padding,
					y * line_height + text_padding,
					cursor_thickness,
					line_height
				),
				cursor_color
			)
		);
	}

	return queue;
}
}

Signal::SignalID MultiLineEntry::OnTextChanged = 0;

MultiLineEntry::MultiLineEntry() :
	enteredText(),
	cursorX(0),
	cursorY(0),
	elapsedTime(0.f),
	textMargin(0.f),
	cursorStatus(false)
{
}

MultiLineEntry::Ptr MultiLineEntry::Create(const sf::String& text) {
	Ptr ptr(new MultiLineEntry());
	ptr->SetText(text);
	return ptr;
}

unique_ptr<RenderQueue> MultiLineEntry::InvalidateImpl() const {
    sf::String line = enteredText.size() > 0 ? enteredText[cursorY] : "";
    if (signed(line.getSize()) > cursorX)
        line.erase(cursorX, line.getSize()-cursorX);
	return eng::createEntryDrawable(
        dynamic_pointer_cast<const MultiLineEntry>(shared_from_this()), line, cursorY);
}

void MultiLineEntry::SetText(const sf::String& text) {
	enteredText.clear();
	sf::String line;
	for (unsigned int i = 0; i<text.getSize(); ++i) {
        if (text[i] == '\n') {
            enteredText.push_back(line);
            line.clear();
        }
        else
            line += text[i];
	}

	cursorX = cursorY = 0;
	Invalidate();
	GetSignals().Emit(OnTextChanged);
}

sf::String MultiLineEntry::GetText() const {
	return getScrolledText();
}

void MultiLineEntry::SetPositionFromMousePos(int mouseX, int mouseY) {
	const string& font_name(Context::Get().GetEngine().GetProperty<string>("FontName", shared_from_this()));
	unsigned int font_size(Context::Get().GetEngine().GetProperty<unsigned int>("FontSize", shared_from_this()));
	const sf::Font& font(*Context::Get().GetEngine().GetResourceManager().GetFont(font_name));
	float text_padding(Context::Get().GetEngine().GetProperty<float>("Padding", shared_from_this()));
    auto line_height = Context::Get().GetEngine().GetFontLineHeight(font, font_size);

	mouseY -= GetAllocation().top + text_padding;
	cursorY = mouseY / line_height;
	if (cursorY < 0)
        cursorY = 0;
    if (cursorY > signed(enteredText.size()))
        cursorY = enteredText.size();

    if (enteredText.size() > 0) {
        basic_string<sf::Uint32> temp(enteredText[cursorY].begin(), enteredText[cursorY].end());
        auto text_start = GetAllocation().left + text_padding;
        auto last_delta = fabs(text_start - static_cast<float>(mouseX));
        cursorX = 0;

        for(cursorX = 0; cursorX < signed(temp.size()); cursorX++) {
            auto text_length = Context::Get().GetEngine().GetTextStringMetrics(
                                    temp.substr(0, static_cast<size_t>(cursorX + 1)), font, font_size).x;
            auto new_delta = fabs(text_start + text_length - static_cast<float>(mouseX));
            if (new_delta < last_delta) {
                last_delta = new_delta;
            }
            else {
                break;
            }
        }
    }
    else
        cursorX = 0;
}

void MultiLineEntry::HandleTextEvent(sf::Uint32 character) {
	if (character > 0x1f && character != 0x7f) {
		if (enteredText.size() == 0)
            enteredText.push_back("");
        if (cursorX < signed(enteredText[cursorY].getSize()))
            enteredText[cursorY].insert(cursorX, character);
        else
            enteredText[cursorY] += character;
		++cursorX;
        Invalidate();
		GetSignals().Emit(OnTextChanged);
	}
}

void MultiLineEntry::HandleKeyEvent(sf::Keyboard::Key key, bool press) {
	if (!press || !HasFocus()) {
		return;
	}

	switch(key) {
	case sf::Keyboard::BackSpace: { // backspace
		if (enteredText.size() > 0) {
            if (cursorX>0) {
                enteredText[cursorY].erase(cursorX-1);
                --cursorX;
                Invalidate();
                GetSignals().Emit(OnTextChanged);
            }
		}
	} break;
	case sf::Keyboard::Delete: {
		if (enteredText.size() > 0) {
            if (cursorX < signed(enteredText[cursorY].getSize()-1)) {
                enteredText[cursorY].erase(cursorX);
                Invalidate();
                GetSignals().Emit(OnTextChanged);
            }
		}
	} break;
	case sf::Keyboard::Home: {
		cursorX = 0;
		Invalidate();
	} break;
	case sf::Keyboard::End: {
		if (enteredText.size() > 0) {
			cursorX = enteredText[cursorY].getSize();
			Invalidate();
		}
	} break;
	case sf::Keyboard::Left: {
		if (cursorX > 0) {
            --cursorX;
            Invalidate();
		}
		else if (cursorY > 0) {
            --cursorY;
            cursorX = enteredText[cursorY].getSize();
            Invalidate();
		}
	} break;
	case sf::Keyboard::Right: {
		if (cursorX < signed(enteredText[cursorY].getSize())) {
            ++cursorX;
            Invalidate();
		}
		else if (cursorY < signed(enteredText.size())) {
            ++cursorY;
            cursorX = 0;
            Invalidate();
		}
	} break;
	case sf::Keyboard::Up: {
        if (cursorY > 0) {
            --cursorY;
            if (cursorY<signed(enteredText.size())) {
                cursorX = min(size_t(cursorX), enteredText[cursorY].getSize());
            }
            Invalidate();
        }
        else {
            cursorX = 0;
            Invalidate();
        }
	} break;
	case sf::Keyboard::Down: {
		if (cursorY < signed(enteredText.size()-1)) {
            ++cursorY;
            cursorX = min(size_t(cursorX), enteredText[cursorY].getSize());
            Invalidate();
		}
		else if (enteredText.size() > 0) {
            cursorX = enteredText[cursorY].getSize();
            Invalidate();
		}
	} break;
	case sf::Keyboard::Return: {
        enteredText.insert(enteredText.begin()+cursorY+1, "");
        ++cursorY;
        cursorX = 0;
		GetSignals().Emit(OnTextChanged);
		Invalidate();
	} break;
	default: break;
	}
}

void MultiLineEntry::HandleMouseEnter(int, int) {
	if (!HasFocus()) {
		SetState(State::PRELIGHT);
	}
}

void MultiLineEntry::HandleMouseLeave(int, int) {
	if (!HasFocus()) {
		SetState(State::NORMAL);
	}
}

void MultiLineEntry::HandleMouseButtonEvent(sf::Mouse::Button button, bool press, int x, int y) {
	if (!press || !IsMouseInWidget()) {
		return;
	}

	if (button != sf::Mouse::Left)
        return;

	GrabFocus();
	SetPositionFromMousePos(x, y);
}

void MultiLineEntry::HandleUpdate(float seconds) {
	if (!HasFocus()) {
		return;
	}

	elapsedTime += seconds;

	// Toggle cursor state every 0.5 seconds
	if (elapsedTime > .5f) {
		elapsedTime = 0.f;
		cursorStatus = !cursorStatus;
		Invalidate();
	}
}

void MultiLineEntry::HandleFocusChange(Widget::Ptr focused_widget) {
	if (HasFocus()) {
		elapsedTime = 0.f;
		cursorStatus = true;
	}

	Invalidate();

	Widget::HandleFocusChange(focused_widget);
}

void MultiLineEntry::HandleSizeChange() {
	if (HasFocus()) {
		GrabFocus(Widget::Ptr());
	}

	cursorX = cursorY = 0;
	Invalidate();
}

sf::Vector2f MultiLineEntry::CalculateRequisition() {
	const string& font_name(Context::Get().GetEngine().GetProperty<string>("FontName", shared_from_this()));
	unsigned int font_size(Context::Get().GetEngine().GetProperty<unsigned int>("FontSize", shared_from_this()));
	float border_width(Context::Get().GetEngine().GetProperty<float>("BorderWidth", shared_from_this()));
	float text_padding(Context::Get().GetEngine().GetProperty<float>("Padding", shared_from_this()));
	const sf::Font& font(*Context::Get().GetEngine().GetResourceManager().GetFont(font_name));
	auto line_height = Context::Get().GetEngine().GetFontLineHeight(font, font_size);

	//TODO - update height
	return sf::Vector2f(2 * (border_width + text_padding), line_height + 2 * (border_width + text_padding));
}

bool MultiLineEntry::IsCursorVisible() const {
	return cursorStatus;
}

const string& MultiLineEntry::GetName() const {
	static const string name("MultiLineEntry");
	return name;
}

void MultiLineEntry::SetTextMargin(float margin) {
	textMargin = margin;
}

sf::String MultiLineEntry::getScrolledText(int topLine, int firstChar, int width, int height) const {
    sf::String result;
    for (int y = topLine; y<signed(enteredText.size()) && ((y-topLine)<=height || height==0); ++y) {
        for (int x = firstChar; x<signed(enteredText[y].getSize()) && ((x-firstChar)<=width || width==0); ++x) {
            result += enteredText[y][x];
        }
        result += "\n";
    }
    return result;
}

}
