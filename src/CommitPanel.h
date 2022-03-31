#pragma once
#include <optional>
#include "Git.h"
#include "Panel.h"
#include "Color.h"
#include "Attr.h"
#include "LineWrap.h"
#include "UTF8.h"
#include "lib/tinyutf8/tinyutf8.h"

namespace UI {

// CommitPanel: a Panel representing a particular git commit
// CommitPanel contains an index indicating the index of the panel/commit within
// its containing branch, where the top/first CommitPanel is index 0
class _CommitPanel : public _Panel, public std::enable_shared_from_this<_CommitPanel> {
public:
    _CommitPanel(const ColorPalette& colors, bool header, int width, Git::Commit commit) :
    _colors(colors) {
        _commit = commit;
        _header = header;
        _id = Git::DisplayStringForId(_commit.id());
        
        Git::Signature sig = _commit.author();
        _time = Git::ShortStringForTime(Git::TimeForGitTime(sig.time()));
        _author = sig.name();
        _message = LineWrap::Wrap(_LineCountMax, width-2*_LineLenInset, _commit.message());
        
        setSize({width, (_header ? 1 : 0) + 3 + (int)_message.size()});
        _drawNeeded = true;
    }
    
    void setBorderColor(std::optional<Color> x) {
        if (_borderColor == x) return;
        _borderColor = x;
        _drawNeeded = true;
    }
    
    void setHeaderLabel(const tiny_utf8::string& x) {
        assert(_header);
        if (_headerLabel == x) return;
        _headerLabel = x;
        _drawNeeded = true;
    }
    
    void draw() {
        const int offY = (_header ? 1 : 0);
        
        int i = 0;
        for (const tiny_utf8::string& line : _message) {
            drawText({2, offY+2+i}, "%s", line.c_str());
            i++;
        }
        
        {
            UI::Attr attr;
            if (_borderColor) attr = Attr(shared_from_this(), *_borderColor);
            drawBorder();
            
            if (_commit.isMerge()) {
                drawText({0, 1}, "%s", "𝝠");
            }
        }
        
        {
            UI::Attr bold(shared_from_this(), A_BOLD);
            UI::Attr color;
            if (!_header && _borderColor) color = Attr(shared_from_this(), *_borderColor);
            drawText({2 + (_header ? -1 : 0), offY+0}, " %s ", _id.c_str());
        }
        
        {
            constexpr int width = 16;
            int off = width - (int)_time.length();
            drawText({12 + (_header ? 1 : 0) + off, offY+0}, " %s ", _time.c_str());
        }
        
        if (_header) {
            UI::Attr attr;
            if (_borderColor) attr = Attr(shared_from_this(), *_borderColor);
            drawText({3, 0}, " %s ", _headerLabel.c_str());
        }
        
        {
            UI::Attr attr(shared_from_this(), _colors.dimmed);
            drawText({2, offY+1}, "%s", _author.c_str());
        }
        
        _drawNeeded = false;
    }
    
    void drawIfNeeded() {
        if (_drawNeeded) {
            draw();
        }
    }
    
    const Git::Commit commit() const { return _commit; }
    
private:
    static constexpr size_t _LineCountMax = 2;
    static constexpr size_t _LineLenInset = 2;
    
    const ColorPalette& _colors;
    Git::Commit _commit;
    bool _header = false;
    tiny_utf8::string _headerLabel;
    tiny_utf8::string _id;
    tiny_utf8::string _time;
    tiny_utf8::string _author;
    std::vector<tiny_utf8::string> _message;
    std::optional<Color> _borderColor;
    bool _drawNeeded = false;
};

using CommitPanel = std::shared_ptr<_CommitPanel>;
using CommitPanelVec = std::vector<CommitPanel>;
using CommitPanelVecIter = CommitPanelVec::iterator;

} // namespace UI
