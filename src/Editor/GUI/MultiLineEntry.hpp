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
    sf::String GetText() const;

    /**
     * Check if cursor is visible.
     * @return true if visible.
     */
    bool IsCursorVisible() const;

    /**
     * Returns the text, but modified to reflect scrolled position
     */
    sf::String getScrolledText(int topLine = 0, int firstChar = 0, int width = 0, int height = 0) const;

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
    void SetPositionFromMousePos(int mouseX, int mouseY);

    void HandleMouseEnter(int x, int y) override;
    void HandleMouseLeave(int x, int y) override;

    std::vector<sf::String> enteredText;
    int cursorX, cursorY;
    float elapsedTime;
    float textMargin;
    mutable bool cursorStatus;
};

}

#endif // MULTILINEENTRY_HPP
