/***

  Olive - Non-Linear Video Editor
  Copyright (C) 2019 Olive Team

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

***/

#include "toolbar.h"

#include <QPushButton>
#include <QVariant>
#include <QButtonGroup>
#include <QEvent>

#include "ui/icons/icons.h"

Toolbar::Toolbar(QWidget *parent) :
  QWidget(parent)
{
  layout_ = new FlowLayout(this);
  layout_->setMargin(0);

  // Create standard tool buttons
  btn_pointer_tool_ = CreateToolButton(olive::tool::kPointer);
  btn_edit_tool_ = CreateToolButton(olive::tool::kEdit);
  btn_ripple_tool_ = CreateToolButton(olive::tool::kRipple);
  btn_rolling_tool_ = CreateToolButton(olive::tool::kRolling);
  btn_razor_tool_ = CreateToolButton(olive::tool::kRazor);
  btn_slip_tool_ = CreateToolButton(olive::tool::kSlip);
  btn_slide_tool_ = CreateToolButton(olive::tool::kSlide);
  btn_hand_tool_ = CreateToolButton(olive::tool::kHand);
  btn_zoom_tool_ = CreateToolButton(olive::tool::kZoom);
  btn_record_ = CreateToolButton(olive::tool::kRecord);
  btn_transition_tool_ = CreateToolButton(olive::tool::kTransition);
  btn_add_ = CreateToolButton(olive::tool::kAdd);

  // Create snapping button, which is not actually a tool, it's a toggle option
  btn_snapping_toggle_ = CreateNonToolButton();
  connect(btn_snapping_toggle_, SIGNAL(clicked(bool)), this, SLOT(SnappingButtonClicked(bool)));

  Retranslate();
  UpdateIcons();
}

void Toolbar::SetTool(const olive::tool::Tool& tool)
{
  // For each tool, set the "checked" state to whether the button's tool is the current tool
  for (int i=0;i<toolbar_btns_.size();i++) {
    ToolbarButton* btn = toolbar_btns_.at(i);

    btn->setChecked(btn->tool() == tool);
  }
}

void Toolbar::SetSnapping(const bool& snapping)
{
  // Set checked state of snapping toggle
  btn_snapping_toggle_->setChecked(snapping);
}

void Toolbar::changeEvent(QEvent *e)
{
  if (e->type() == QEvent::LanguageChange) {
    Retranslate();
  } else if (e->type() == QEvent::StyleChange) {
    UpdateIcons();
  }
  QWidget::changeEvent(e);
}

void Toolbar::Retranslate()
{
  btn_pointer_tool_->setToolTip(tr("Pointer Tool"));
  btn_edit_tool_->setToolTip(tr("Edit Tool"));
  btn_ripple_tool_->setToolTip(tr("Ripple Tool"));
  btn_rolling_tool_->setToolTip(tr("Rolling Tool"));
  btn_razor_tool_->setToolTip(tr("Razor Tool"));
  btn_slip_tool_->setToolTip(tr("Slip Tool"));
  btn_slide_tool_->setToolTip(tr("Slide Tool"));
  btn_hand_tool_->setToolTip(tr("Hand Tool"));
  btn_zoom_tool_->setToolTip(tr("Zoom Tool"));
  btn_transition_tool_->setToolTip(tr("Transition Tool"));
  btn_record_->setToolTip(tr("Record Tool"));
  btn_add_->setToolTip(tr("Add Tool"));
  btn_snapping_toggle_->setToolTip(tr("Toggle Snapping"));
}

void Toolbar::UpdateIcons()
{
  btn_pointer_tool_->setIcon(olive::icon::ToolPointer);
  btn_edit_tool_->setIcon(olive::icon::ToolEdit);
  btn_ripple_tool_->setIcon(olive::icon::ToolRipple);
  btn_rolling_tool_->setIcon(olive::icon::ToolRolling);
  btn_razor_tool_->setIcon(olive::icon::ToolRazor);
  btn_slip_tool_->setIcon(olive::icon::ToolSlip);
  btn_slide_tool_->setIcon(olive::icon::ToolSlide);
  btn_hand_tool_->setIcon(olive::icon::ToolHand);
  btn_zoom_tool_->setIcon(olive::icon::ZoomIn);
  btn_record_->setIcon(olive::icon::Record);
  btn_transition_tool_->setIcon(olive::icon::ToolTransition);
  btn_add_->setIcon(olive::icon::Add);
  btn_snapping_toggle_->setIcon(olive::icon::Snapping);
}

ToolbarButton* Toolbar::CreateToolButton(const olive::tool::Tool& tool)
{
  // Create a ToolbarButton object
  ToolbarButton* b = new ToolbarButton(this, tool);

  // Add it to the layout
  layout_->addWidget(b);

  // Add it to the list for iterating through later
  toolbar_btns_.append(b);

  // Connect it to the tool button click handler
  connect(b, SIGNAL(clicked(bool)), this, SLOT(ToolButtonClicked()));

  return b;
}

ToolbarButton *Toolbar::CreateNonToolButton()
{
  // Create a ToolbarButton object
  ToolbarButton* b = new ToolbarButton(this, olive::tool::kNone);

  // Add it to the layout
  layout_->addWidget(b);

  return b;
}

void Toolbar::ToolButtonClicked()
{
  // Get new tool from ToolbarButton object
  olive::tool::Tool new_tool = static_cast<ToolbarButton*>(sender())->tool();

  // Set checked state of all tool buttons
  // NOTE: Not necessary if this is appropriately connected to Core
  //SetTool(new_tool);

  // Emit signal that the tool just changed
  emit ToolChanged(new_tool);
}

void Toolbar::SnappingButtonClicked(bool b)
{
  emit SnappingChanged(b);
}
