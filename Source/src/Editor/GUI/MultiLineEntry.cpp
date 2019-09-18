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
unique_ptr<RenderQueue> createEntryDrawable(shared_ptr<const MultiLineEntry> entry) {
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
        sf::String text(entry->GetText());
		/*if (entry->GetCursorPosition() - entry->GetVisibleOffset() < static_cast<int>(text.getSize())) {
			text.erase(static_cast<std::size_t>(entry->GetCursorPosition() - entry->GetVisibleOffset()), text.getSize());
		}*/

		int y = 0;
		sf::String temp;
        for (unsigned int i = 0; signed(i)<entry->GetCursorPosition() && i<text.getSize(); ++i) {
            if (text[i] == '\n') {
                ++y;
                temp.clear();
            }
            else
                temp += text[i];
        }
		sf::Vector2f metrics(e.GetTextStringMetrics(temp, *font, font_size));

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
	cursorPos(0),
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
	return eng::createEntryDrawable(
        dynamic_pointer_cast<const MultiLineEntry>(shared_from_this()));
}

void MultiLineEntry::SetText(const sf::String& text) {
	enteredText = text;
	cursorPos = 0;
	GetSignals().Emit(OnTextChanged);
}

const sf::String& MultiLineEntry::GetText() const {
	return enteredText;
}

void MultiLineEntry::SetCursorPosition(int new_position) {
	if (new_position == cursorPos) {
		return;
	}

	int delta = new_position - cursorPos;
	MoveCursor(delta);
}

int MultiLineEntry::GetPositionFromMousePos(int mouseX, int mouseY) {
	const string& font_name(Context::Get().GetEngine().GetProperty<string>("FontName", shared_from_this()));
	unsigned int font_size(Context::Get().GetEngine().GetProperty<unsigned int>("FontSize", shared_from_this()));
	const sf::Font& font(*Context::Get().GetEngine().GetResourceManager().GetFont(font_name));
	float text_padding(Context::Get().GetEngine().GetProperty<float>("Padding", shared_from_this()));
    auto line_height = Context::Get().GetEngine().GetFontLineHeight(font, font_size);

	basic_string<sf::Uint32> temp;

	mouseY -= GetAllocation().top + text_padding;
	int y = 1;
	unsigned int offset = 0;
	for (; offset<enteredText.getSize(); ++offset) {
        if (enteredText[offset] == '\n') {
            ++y;
            temp.clear();
        }
        else
            temp.push_back(enteredText[offset]);

        if (y * line_height >= mouseY) {
            unsigned int i = offset + 1;
            while (enteredText[i] != '\n' && i < enteredText.getSize()) {
                temp.push_back(enteredText[i]);
                ++i;
            }
            break;
        }
	}

	auto text_start = GetAllocation().left + text_padding;
	auto last_delta = fabs(text_start - static_cast<float>(mouseX));
	int cursor_position = 0;
	auto length = static_cast<int>(temp.size());

	//TODO - update
	for(cursor_position = 0; cursor_position < length; cursor_position++) {
		auto text_length = Context::Get().GetEngine().GetTextStringMetrics(temp.substr(0, static_cast<size_t>(cursor_position + 1)), font, font_size).x;
		auto new_delta = fabs(text_start + text_length - static_cast<float>(mouseX));
		if (new_delta < last_delta) {
			last_delta = new_delta;
		}
		else {
			break;
		}
	}

	return cursor_position + offset;
}

void MultiLineEntry::MoveCursor(int delta) {
	if (delta && (cursorPos + delta >= 0) && (cursorPos + delta <= signed(enteredText.getSize()))) {
		cursorPos += delta;

		// Make cursor visible.
		elapsedTime = 0.f;
		cursorStatus = true;

		Invalidate();
	}
}

void MultiLineEntry::HandleTextEvent(sf::Uint32 character) {
	if (character > 0x1f && character != 0x7f) {
		// not a control character
		enteredText.insert(cursorPos, character);
		MoveCursor(1);

		GetSignals().Emit(OnTextChanged);
	}
}

void MultiLineEntry::HandleKeyEvent(sf::Keyboard::Key key, bool press) {
	if (!press || !HasFocus()) {
		return;
	}

	switch(key) {
	case sf::Keyboard::BackSpace: { // backspace
		if ((enteredText.getSize() > 0) && (cursorPos > 0)) {
			enteredText.erase(static_cast<size_t>(cursorPos - 1));
			MoveCursor(-1);
			elapsedTime = 0.f;
			cursorStatus = true;
			GetSignals().Emit(OnTextChanged);
		}
	} break;
	case sf::Keyboard::Delete: {
		if ((enteredText.getSize() > 0) && (cursorPos < static_cast<int>(enteredText.getSize()))) {
			enteredText.erase(static_cast<size_t>(cursorPos));
			elapsedTime = 0.f;
			cursorStatus = true;
			GetSignals().Emit(OnTextChanged);
			Invalidate();
		}
	} break;
	case sf::Keyboard::Home: {
		if (enteredText.getSize() > 0) {
			int c = GetCursorPosition();
			if (c > 0 && enteredText[c] == '\n')
                --c;
			while (c > 0 && enteredText[c] != '\n')
                --c;
            if (c > 0)
                ++c;
            SetCursorPosition(c);
		}
	} break;
	case sf::Keyboard::End: {
		if (enteredText.getSize() > 0) {
			int c = GetCursorPosition();
            while (enteredText[c] != '\n' && c < signed(enteredText.getSize()))
                ++c;
            SetCursorPosition(c);
		}
	} break;
	case sf::Keyboard::Left: {
		MoveCursor(-1);
	} break;
	case sf::Keyboard::Right: {
		MoveCursor(1);
	} break;
	case sf::Keyboard::Up: {
		int c = GetCursorPosition();
		int x = 0;
		while (enteredText[c] != '\n' && c > 0) {
            --c;
            ++x;
		}
		int w = 0;
		if (c > 0) {
            --c;
            if (enteredText[c] == '\n' && c > 0)
                --c;
            while (enteredText[c] != '\n' && c > 0) {
                --c;
                ++w;
            }
		}
		x = min(x, w);
		SetCursorPosition(c+x);
	} break;
	case sf::Keyboard::Down: {
		int c = GetCursorPosition();
		int x = 0;
		while (enteredText[c] != '\n' && c > 0) {
            --c;
            ++x;
		}
		c = GetCursorPosition();
		int o = 0;
		while (enteredText[c] != '\n' && c < signed(enteredText.getSize())) {
            ++c;
            ++o;
		}
		int w = 0;
		if (c < signed(enteredText.getSize())) {
            ++c;
            while (enteredText[c] != '\n' && c < signed(enteredText.getSize())) {
                ++c;
                ++w;
            }
            c -= w;
		}
		if (o > 0)
            o = 1;
		x = min(x, w);
		SetCursorPosition(c+x-o);
	} break;
	case sf::Keyboard::Return: {
        enteredText.insert(cursorPos, '\n');
		MoveCursor(1);
		GetSignals().Emit(OnTextChanged);
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
	SetCursorPosition(GetPositionFromMousePos(x, y));
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

	SetCursorPosition(0);
}

sf::Vector2f MultiLineEntry::CalculateRequisition() {
	const string& font_name(Context::Get().GetEngine().GetProperty<string>("FontName", shared_from_this()));
	unsigned int font_size(Context::Get().GetEngine().GetProperty<unsigned int>("FontSize", shared_from_this()));
	float border_width(Context::Get().GetEngine().GetProperty<float>("BorderWidth", shared_from_this()));
	float text_padding(Context::Get().GetEngine().GetProperty<float>("Padding", shared_from_this()));
	const sf::Font& font(*Context::Get().GetEngine().GetResourceManager().GetFont(font_name));
	auto line_height = Context::Get().GetEngine().GetFontLineHeight(font, font_size);

	return sf::Vector2f(2 * (border_width + text_padding), line_height + 2 * (border_width + text_padding));
}

bool MultiLineEntry::IsCursorVisible() const {
	return cursorStatus;
}

int MultiLineEntry::GetCursorPosition() const {
	return cursorPos;
}

const string& MultiLineEntry::GetName() const {
	static const string name("MultiLineEntry");
	return name;
}

void MultiLineEntry::SetTextMargin(float margin) {
	textMargin = margin;
}

}
