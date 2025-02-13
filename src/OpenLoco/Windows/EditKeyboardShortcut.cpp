#include "../Config.h"
#include "../Graphics/Colour.h"
#include "../Graphics/ImageIds.h"
#include "../Input/Shortcut.h"
#include "../Input/ShortcutManager.h"
#include "../Interop/Interop.hpp"
#include "../Localisation/FormatArguments.hpp"
#include "../Localisation/StringIds.h"
#include "../Objects/InterfaceSkinObject.h"
#include "../Objects/ObjectManager.h"
#include "../Ui/WindowManager.h"

using namespace OpenLoco::Interop;
using namespace OpenLoco::Input;

namespace OpenLoco::Ui::Windows::EditKeyboardShortcut
{
    constexpr Gfx::ui_size_t windowSize = { 280, 72 };

    static window_event_list events;
    static loco_global<uint8_t, 0x011364A4> _11364A4;

    static widget_t _widgets[] = {
        makeWidget({ 0, 0 }, windowSize, widget_type::frame, 0, 0xFFFFFFFF),                                                 // 0,
        makeWidget({ 1, 1 }, { windowSize.width - 2, 13 }, widget_type::caption_25, 0, StringIds::change_keyboard_shortcut), // 1,
        makeWidget({ 265, 2 }, { 13, 13 }, widget_type::wt_9, 0, ImageIds::close_button, StringIds::tooltip_close_window),   // 2,
        makeWidget({ 0, 15 }, { windowSize.width, 57 }, widget_type::panel, 1, 0xFFFFFFFF),                                  // 3,
        widgetEnd(),
    };

    static void initEvents();

    namespace Widx
    {
        enum
        {
            frame,
            caption,
            close,
            panel,
        };
    }

    // 0x004BF7B9
    window* open(const uint8_t shortcutIndex)
    {
        WindowManager::close(WindowType::editKeyboardShortcut);
        _11364A4 = shortcutIndex;

        // TODO: only needs to be called once
        initEvents();

        auto window = WindowManager::createWindow(WindowType::editKeyboardShortcut, windowSize, 0, &events);

        window->widgets = _widgets;
        window->enabled_widgets = 1 << Widx::close;
        window->initScrollWidgets();

        const auto skin = ObjectManager::get<InterfaceSkinObject>();
        window->colours[0] = skin->colour_0B;
        window->colours[1] = skin->colour_10;

        return window;
    }

    // 0x004BE8DF
    static void draw(Ui::window* const self, Gfx::drawpixelinfo_t* const ctx)
    {
        self->draw(ctx);

        FormatArguments args{};
        args.push(ShortcutManager::getName(static_cast<Shortcut>(*_11364A4)));
        auto point = Gfx::point_t(self->x + 140, self->y + 32);
        Gfx::drawStringCentredWrapped(ctx, &point, 272, 0, StringIds::change_keyboard_shortcut_desc, &args);
    }

    // 0x004BE821
    static void onMouseUp(window* const self, const widget_index widgetIndex)
    {
        switch (widgetIndex)
        {
            case Widx::close:
                WindowManager::close(self);
                return;
        }
    }

    static void initEvents()
    {
        events.on_mouse_up = onMouseUp;
        events.draw = draw;
    }
}
