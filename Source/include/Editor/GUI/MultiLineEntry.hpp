#ifndef MULTILINEENTRY_HPP
#define MULTILINEENTRY_HPP

#include <SFGUI/Widget.hpp>

#include <SFML/System/String.hpp>
#include <memory>

namespace sfg {

/**
 * MultiLineEntry widget
 */
class MultiLineEntry : public Widget {
public:
    typedef std::shared_ptr<MultiLineEntry> Ptr; //!< Shared pointer.
    typedef std::shared_ptr<const MultiLineEntry> PtrConst; //!< Shared pointer.

    /**
     * Create entry.
     * @param text Text.
     * @return MultiLineEntry.
     */
    static Ptr Create(const sf::String& text = L"");

    const std::string& GetName() const override;

    /**
     * Set text.
     * @param text Text.
     */
    void SetText(const sf::String& text);

    /**
     * Get text.
     * @return Text.
     */
    const sf::String& GetText() const;

    /**
     * Get current cursor position.
     * @return Cursor position.
     */
    int GetCursorPosition() const;

    /**
     * Set cursor position.
     * @param pos Position.
     */
    void SetCursorPosition(int pos);

    /**
     * Check if cursor is visible.
     * @return true if visible.
     */
    bool IsCursorVisible() const;

    static Signal::SignalID OnTextChanged; //!< Fired when the text changes.

protected:
    /**
     * Ctor.
     */
    MultiLineEntry();

    /**
     * Set right hand side margin that the entry should reserve for derived widgets.
     * @param margin Margin to reserve.
     */
    void SetTextMargin(float margin);

    std::unique_ptr<RenderQueue> InvalidateImpl() const override;
    sf::Vector2f CalculateRequisition() override;

    void HandleMouseButtonEvent(sf::Mouse::Button button, bool press, int x, int y) override;
    void HandleUpdate(float seconds) override;
    void HandleTextEvent(sf::Uint32 character) override;
    void HandleKeyEvent(sf::Keyboard::Key key, bool press) override;
    void HandleSizeChange() override;
    void HandleFocusChange(Widget::Ptr focused_widget) override;

private:
    /**
     * Get closest cursor position to coordinate.
     * @param mouseX x coordinate.
     * @param mouseY y coordinate
     * @return Closest cursor position.
     */
    int GetPositionFromMousePos(int mouseX, int mouseY);

    /**
     * Move cursor.
     * @param delta Number of units to move cursor by. Negative to move left. Positive to move right.
     */
    void MoveCursor(int delta);

    void HandleMouseEnter(int x, int y) override;
    void HandleMouseLeave(int x, int y) override;

    sf::String enteredText;
    int cursorPos;
    float elapsedTime;
    float textMargin;
    mutable bool cursorStatus;
};

}

#endif // MULTILINEENTRY_HPP
