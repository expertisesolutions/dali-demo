/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file style-example.cpp
 * @brief Example of styling Toolkit controls.
 */

// Class include
#include "styling-application.h"

// External includes
#include <dali-toolkit/dali-toolkit.h>
#include <dali-toolkit/devel-api/controls/popup/popup.h>
#include <dali-toolkit/devel-api/controls/table-view/table-view.h>
#include <dali/devel-api/actors/actor-devel.h>
#include <cstdio>
#include <sstream>
#include "image-channel-control.h"

// Internal includes

using namespace Dali;
using namespace Dali::Toolkit;

namespace Demo
{
const char* StylingApplication::DEMO_THEME_ONE_PATH(DEMO_STYLE_DIR "style-example-theme-one.json");
const char* StylingApplication::DEMO_THEME_TWO_PATH(DEMO_STYLE_DIR "style-example-theme-two.json");
const char* StylingApplication::DEMO_THEME_THREE_PATH(DEMO_STYLE_DIR "style-example-theme-three.json");

namespace
{
#define DP(x) x

const char* DEFAULT_CONTROL_AREA_IMAGE_PATH(DEMO_IMAGE_DIR "popup_button_background.9.png");

const char* POPUP_CONTROL_OK_NAME("PopupControlOk");
const char* POPUP_CONTROL_CANCEL_NAME("PopupControlCancel");
const char* BORDER_IMAGE(DEMO_IMAGE_DIR "border-4px.9.png");
const char* RESIZE_HANDLE_IMAGE(DEMO_IMAGE_DIR "resize-handle.png");

const int NUMBER_OF_THEMES(3); // The default theme is considered.

const Vector4 BACKGROUND_COLOUR(1.0f, 1.0f, 1.0f, 0.15f);
const int     BORDER_WIDTH(4);

const char* const SMALL_IMAGE_1 = DEMO_IMAGE_DIR "gallery-small-14.jpg";
const char* const BIG_IMAGE_1   = DEMO_IMAGE_DIR "gallery-large-4.jpg";

const char* const SMALL_IMAGE_2 = DEMO_IMAGE_DIR "gallery-small-39.jpg";
const char* const BIG_IMAGE_2   = DEMO_IMAGE_DIR "gallery-large-7.jpg";

const char* const SMALL_IMAGE_3 = DEMO_IMAGE_DIR "gallery-small-20.jpg";
const char* const BIG_IMAGE_3   = DEMO_IMAGE_DIR "gallery-large-11.jpg";

// Layout
const int MARGIN_SIZE = 10;

const int RADIO_LABEL_THUMBNAIL_SIZE       = 60;
const int RADIO_LABEL_THUMBNAIL_SIZE_SMALL = 40;
const int RADIO_IMAGE_SPACING              = 8;
const int BUTTON_HEIGHT                    = 48;

Property::Index GetChannelProperty(int index)
{
  Property::Index channelIndex = Property::INVALID_INDEX;
  switch(index)
  {
    case 0:
    {
      channelIndex = ImageChannelControl::Property::RED_CHANNEL;
      break;
    }
    case 1:
    {
      channelIndex = ImageChannelControl::Property::GREEN_CHANNEL;
      break;
    }
    case 2:
    {
      channelIndex = ImageChannelControl::Property::BLUE_CHANNEL;
      break;
    }
  }
  return channelIndex;
}

} // anonymous namespace

StylingApplication::StylingApplication(Application& application)
: mApplication(application),
  mCurrentTheme(0)
{
  application.InitSignal().Connect(this, &StylingApplication::Create);
}

StylingApplication::~StylingApplication()
{
}

void StylingApplication::Create(Application& application)
{
  Window window = application.GetWindow();
  window.KeyEventSignal().Connect(this, &StylingApplication::OnKeyEvent);
  window.SetBackgroundColor(Vector4(0.1f, 0.1f, 0.1f, 1.0f));

  mContentPane = CreateContentPane();
  window.Add(mContentPane);
  mContentPane.SetProperty(Actor::Property::SIZE, Vector2(window.GetSize()));

  // Content panes:
  TableView contentLayout = TableView::New(5, 1);
  contentLayout.SetProperty(Dali::Actor::Property::NAME, "ContentLayout");
  contentLayout.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::ALL_DIMENSIONS);
  contentLayout.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT);
  contentLayout.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT);
  contentLayout.SetCellPadding(Size(MARGIN_SIZE, MARGIN_SIZE));

  // Assign all rows the size negotiation property of fitting to children
  for(unsigned int i = 0; i < contentLayout.GetRows(); ++i)
  {
    if(i != 1)
    {
      contentLayout.SetFitHeight(i); // Row 1 should fill
    }
  }

  mContentPane.Add(contentLayout);

  mTitle = TextLabel::New("Styling Example");
  mTitle.SetProperty(Dali::Actor::Property::NAME, "Title");
  mTitle.SetStyleName("Title");
  mTitle.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_CENTER);
  mTitle.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_CENTER);
  mTitle.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::WIDTH);
  mTitle.SetResizePolicy(ResizePolicy::USE_NATURAL_SIZE, Dimension::HEIGHT);
  mTitle.SetProperty(TextLabel::Property::HORIZONTAL_ALIGNMENT, "CENTER");
  contentLayout.Add(mTitle);

  // Buttons:

  TableView imageSelectLayout = TableView::New(1, 2);
  imageSelectLayout.SetProperty(Dali::Actor::Property::NAME, "ImageSelectLayout");

  imageSelectLayout.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::WIDTH);
  imageSelectLayout.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::HEIGHT);
  imageSelectLayout.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
  imageSelectLayout.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
  imageSelectLayout.SetCellPadding(Size(MARGIN_SIZE, MARGIN_SIZE));

  // Fit radio button column to child width, leave image to fill remainder
  imageSelectLayout.SetFitWidth(0);

  contentLayout.Add(imageSelectLayout);

  TableView radioButtonsLayout = TableView::New(3, 2);
  radioButtonsLayout.SetProperty(Dali::Actor::Property::NAME, "RadioButtonsLayout");
  radioButtonsLayout.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::HEIGHT);
  // Leave each row to fill to parent height
  // Set each column to fit to child width
  radioButtonsLayout.SetFitWidth(0);
  radioButtonsLayout.SetFitWidth(1);
  radioButtonsLayout.SetCellPadding(Vector2(6.0f, 0.0f));

  imageSelectLayout.AddChild(radioButtonsLayout, TableView::CellPosition(0, 0));
  imageSelectLayout.SetCellAlignment(TableView::CellPosition(0, 0), HorizontalAlignment::LEFT, VerticalAlignment::CENTER);

  const char* images[] = {SMALL_IMAGE_1, SMALL_IMAGE_2, SMALL_IMAGE_3};

  for(int i = 0; i < 3; ++i)
  {
    std::ostringstream thumbnailName;
    thumbnailName << "thumbnail" << i + 1;
    ImageView image = ImageView::New(images[i]);
    image.SetProperty(Dali::Actor::Property::NAME, thumbnailName.str());
    image.SetProperty(Actor::Property::SIZE, Vector2(DP(RADIO_LABEL_THUMBNAIL_SIZE), DP(RADIO_LABEL_THUMBNAIL_SIZE)));

    std::ostringstream label;
    label << (i + 1);
    std::ostringstream radioButtonStyleName;
    radioButtonStyleName << "imageSelectButton" << i + 1;
    mRadioButtons[i] = RadioButton::New(label.str());
    mRadioButtons[i].SetProperty(Dali::Actor::Property::NAME, radioButtonStyleName.str());
    mRadioButtons[i].SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_LEFT);
    mRadioButtons[i].SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_LEFT);
    mRadioButtons[i].SetProperty(Button::Property::SELECTED, false);
    mRadioButtons[i].StateChangedSignal().Connect(this, &StylingApplication::OnButtonStateChange);

    radioButtonsLayout.AddChild(mRadioButtons[i], TableView::CellPosition(i, 0));
    radioButtonsLayout.AddChild(image, TableView::CellPosition(i, 1));
    radioButtonsLayout.SetCellAlignment(TableView::CellPosition(i, 0), HorizontalAlignment::CENTER, VerticalAlignment::CENTER);
    radioButtonsLayout.SetCellAlignment(TableView::CellPosition(i, 1), HorizontalAlignment::CENTER, VerticalAlignment::CENTER);
  }

  mRadioButtons[0].SetProperty(Button::Property::SELECTED, true);

  mImagePlacement = Actor::New();
  mImagePlacement.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
  mImagePlacement.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
  mImagePlacement.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::ALL_DIMENSIONS);
  imageSelectLayout.AddChild(mImagePlacement, TableView::CellPosition(0, 1));
  imageSelectLayout.SetCellAlignment(TableView::CellPosition(0, 1), HorizontalAlignment::RIGHT, VerticalAlignment::CENTER);

  mIcc1 = ImageChannelControl::New(BIG_IMAGE_1);
  mIcc1.SetProperty(Dali::Actor::Property::NAME, "ICC1");
  mIcc1.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::ALL_DIMENSIONS);
  mIcc1.SetProperty(Actor::Property::SIZE_SCALE_POLICY, SizeScalePolicy::FIT_WITH_ASPECT_RATIO);
  mIcc1.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
  mIcc1.SetVisibility(true);

  mImagePlacement.Add(mIcc1);

  mIcc2 = ImageChannelControl::New(BIG_IMAGE_2);
  mIcc2.SetProperty(Dali::Actor::Property::NAME, "ICC2");
  mIcc2.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::ALL_DIMENSIONS);
  mIcc2.SetProperty(Actor::Property::SIZE_SCALE_POLICY, SizeScalePolicy::FIT_WITH_ASPECT_RATIO);
  mIcc2.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
  mIcc2.SetVisibility(false);

  mImagePlacement.Add(mIcc2);

  mIcc3 = ImageChannelControl::New(BIG_IMAGE_3);
  mIcc3.SetProperty(Dali::Actor::Property::NAME, "ICC3");
  mIcc3.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::ALL_DIMENSIONS);
  mIcc3.SetProperty(Actor::Property::SIZE_SCALE_POLICY, SizeScalePolicy::FIT_WITH_ASPECT_RATIO);
  mIcc3.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
  mIcc3.SetVisibility(false);

  mImagePlacement.Add(mIcc3);

  mImageChannelControl = mIcc1;

  TableView channelSliderLayout = TableView::New(3, 3);
  channelSliderLayout.SetProperty(Dali::Actor::Property::NAME, "ChannelSliderLayout");

  // Contains a column of check buttons and a column of sliders for R/G/B
  channelSliderLayout.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::WIDTH);
  channelSliderLayout.SetResizePolicy(ResizePolicy::USE_NATURAL_SIZE, Dimension::HEIGHT);
  channelSliderLayout.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
  channelSliderLayout.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
  channelSliderLayout.SetCellPadding(Size(MARGIN_SIZE, MARGIN_SIZE));

  // Set each row to fit to child height
  channelSliderLayout.SetFitHeight(0);
  channelSliderLayout.SetFitHeight(1);
  channelSliderLayout.SetFitHeight(2);

  // Set each column to fit to child width
  channelSliderLayout.SetFitWidth(0);
  channelSliderLayout.SetFitWidth(1);

  contentLayout.Add(channelSliderLayout);
  const char* checkboxLabels[3] = {"R", "G", "B"};

  for(int i = 0; i < 3; ++i)
  {
    std::ostringstream checkBoxStyleName;
    checkBoxStyleName << "channelActiveCheckBox" << i + 1;
    mCheckButtons[i] = CheckBoxButton::New();
    mCheckButtons[i].SetProperty(Dali::Actor::Property::NAME, checkBoxStyleName.str());
    mCheckButtons[i].SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
    mCheckButtons[i].SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
    mCheckButtons[i].SetProperty(Button::Property::SELECTED, true);

    mCheckButtons[i].StateChangedSignal().Connect(this, &StylingApplication::OnCheckButtonChange);
    mCheckButtons[i].RegisterProperty("channel", i, Property::READ_WRITE);

    channelSliderLayout.AddChild(mCheckButtons[i], TableView::CellPosition(i, 0));
    channelSliderLayout.SetCellAlignment(TableView::CellPosition(i, 0), HorizontalAlignment::CENTER, VerticalAlignment::CENTER);

    TextLabel          label = TextLabel::New(checkboxLabels[i]);
    std::ostringstream labelStyleName;
    labelStyleName << "ColorLabel" << i + 1;
    label.SetProperty(Dali::Actor::Property::NAME, labelStyleName.str());
    label.SetStyleName(labelStyleName.str());
    label.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
    label.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
    label.SetResizePolicy(ResizePolicy::USE_NATURAL_SIZE, Dimension::WIDTH);
    label.SetResizePolicy(ResizePolicy::USE_NATURAL_SIZE, Dimension::HEIGHT);

    channelSliderLayout.AddChild(label, TableView::CellPosition(i, 1));
    channelSliderLayout.SetCellAlignment(TableView::CellPosition(i, 1), HorizontalAlignment::CENTER, VerticalAlignment::CENTER);

    mChannelSliders[i] = Slider::New();
    std::ostringstream sliderStyleName;
    sliderStyleName << "ColorSlider" << i + 1;
    mChannelSliders[i].SetProperty(Dali::Actor::Property::NAME, sliderStyleName.str());
    mChannelSliders[i].SetStyleName(sliderStyleName.str());
    mChannelSliders[i].SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
    mChannelSliders[i].SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
    mChannelSliders[i].SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::WIDTH);
    mChannelSliders[i].SetResizePolicy(ResizePolicy::FIT_TO_CHILDREN, Dimension::HEIGHT);
    mChannelSliders[i].SetProperty(Slider::Property::LOWER_BOUND, 0.0f);
    mChannelSliders[i].SetProperty(Slider::Property::UPPER_BOUND, 100.0f);
    mChannelSliders[i].SetProperty(Slider::Property::VALUE_PRECISION, 0);
    mChannelSliders[i].SetProperty(Slider::Property::VALUE, 100.0f);
    mChannelSliders[i].SetProperty(Slider::Property::SHOW_POPUP, true);
    mChannelSliders[i].SetProperty(Slider::Property::SHOW_VALUE, true);

    mChannelSliders[i].RegisterProperty("channel", i, Property::READ_WRITE);

    channelSliderLayout.AddChild(mChannelSliders[i], TableView::CellPosition(i, 2));
    channelSliderLayout.SetCellAlignment(TableView::CellPosition(i, 2), HorizontalAlignment::RIGHT, VerticalAlignment::CENTER);

    mChannelSliders[i].ValueChangedSignal().Connect(this, &StylingApplication::OnSliderChanged);
  }

  mResetButton = PushButton::New();
  mResetButton.SetProperty(Toolkit::Button::Property::LABEL, "Reset");
  mResetButton.SetProperty(Dali::Actor::Property::NAME, "ResetButton");
  mResetButton.SetResizePolicy(ResizePolicy::USE_NATURAL_SIZE, Dimension::ALL_DIMENSIONS);
  mResetButton.ClickedSignal().Connect(this, &StylingApplication::OnResetClicked);

  contentLayout.Add(mResetButton);
  contentLayout.SetCellAlignment(TableView::CellPosition(3, 0), HorizontalAlignment::CENTER, VerticalAlignment::CENTER);

  TableView themeButtonLayout = TableView::New(1, 4);
  themeButtonLayout.SetProperty(Dali::Actor::Property::NAME, "ThemeButtonsLayout");
  themeButtonLayout.SetCellPadding(Vector2(6.0f, 0.0f));

  themeButtonLayout.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
  themeButtonLayout.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
  themeButtonLayout.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::WIDTH);
  themeButtonLayout.SetResizePolicy(ResizePolicy::FIT_TO_CHILDREN, Dimension::HEIGHT);
  themeButtonLayout.SetCellPadding(Size(MARGIN_SIZE, MARGIN_SIZE));
  themeButtonLayout.SetFitHeight(0);

  TextLabel label = TextLabel::New("Theme: ");
  label.SetResizePolicy(ResizePolicy::USE_NATURAL_SIZE, Dimension::ALL_DIMENSIONS);
  label.SetStyleName("ThemeLabel");
  label.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_CENTER);
  label.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_CENTER);
  themeButtonLayout.AddChild(label, TableView::CellPosition(0, 0));
  themeButtonLayout.SetCellAlignment(TableView::CellPosition(0, 0), HorizontalAlignment::LEFT, VerticalAlignment::CENTER);

  for(int i = 0; i < 3; ++i)
  {
    mThemeButtons[i] = PushButton::New();
    mThemeButtons[i].SetProperty(Dali::Actor::Property::NAME, "ThemeButton");
    mThemeButtons[i].SetStyleName("ThemeButton");
    mThemeButtons[i].SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
    mThemeButtons[i].SetProperty(Actor::Property::ANCHOR_POINT, ParentOrigin::CENTER);
    mThemeButtons[i].SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::WIDTH);
    mThemeButtons[i].SetResizePolicy(ResizePolicy::USE_NATURAL_SIZE, Dimension::HEIGHT);
    mThemeButtons[i].RegisterProperty("theme", i, Property::READ_WRITE);
    mThemeButtons[i].ClickedSignal().Connect(this, &StylingApplication::OnThemeButtonClicked);
    themeButtonLayout.AddChild(mThemeButtons[i], TableView::CellPosition(0, 1 + i));
  }
  mThemeButtons[0].SetProperty(Toolkit::Button::Property::LABEL, "Lite"); // Lightweight changes on top of Dali
  mThemeButtons[1].SetProperty(Toolkit::Button::Property::LABEL, "App1"); // Different application style
  mThemeButtons[2].SetProperty(Toolkit::Button::Property::LABEL, "App2");

  contentLayout.Add(themeButtonLayout);
}

Actor StylingApplication::CreateContentPane()
{
  Toolkit::ImageView contentPane = Toolkit::ImageView::New(BORDER_IMAGE);
  contentPane.SetProperty(Dali::Actor::Property::NAME, "ContentPane");
  contentPane.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
  contentPane.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
  contentPane.SetProperty(Actor::Property::PADDING, Padding(BORDER_WIDTH, BORDER_WIDTH, BORDER_WIDTH, BORDER_WIDTH));
  return contentPane;
}

Actor StylingApplication::CreateResizableContentPane()
{
  Toolkit::ImageView contentPane = Toolkit::ImageView::New(BORDER_IMAGE);
  contentPane.SetProperty(Dali::Actor::Property::NAME, "ContentPane");
  contentPane.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
  contentPane.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
  contentPane.SetProperty(Actor::Property::PADDING, Padding(BORDER_WIDTH, BORDER_WIDTH, BORDER_WIDTH, BORDER_WIDTH));

  Toolkit::ImageView grabHandle = Toolkit::ImageView::New(RESIZE_HANDLE_IMAGE);
  grabHandle.SetProperty(Dali::Actor::Property::NAME, "GrabHandle");
  grabHandle.SetResizePolicy(ResizePolicy::USE_NATURAL_SIZE, Dimension::ALL_DIMENSIONS);
  grabHandle.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::BOTTOM_RIGHT);
  grabHandle.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::BOTTOM_RIGHT);
  grabHandle.SetProperty(Actor::Property::POSITION, Vector2(-BORDER_WIDTH, -BORDER_WIDTH));
  grabHandle.SetProperty(Actor::Property::OPACITY, 0.6f);

  Layer grabCornerLayer = Layer::New();
  grabCornerLayer.SetProperty(Dali::Actor::Property::NAME, "GrabCornerLayer");
  grabCornerLayer.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::BOTTOM_RIGHT);
  grabCornerLayer.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::BOTTOM_RIGHT);
  grabCornerLayer.Add(grabHandle);
  contentPane.Add(grabCornerLayer);

  mPanGestureDetector = PanGestureDetector::New();
  mPanGestureDetector.Attach(grabHandle);
  mPanGestureDetector.DetectedSignal().Connect(this, &StylingApplication::OnPan);

  return contentPane;
}

Popup StylingApplication::CreateResetPopup()
{
  Window window = mApplication.GetWindow();

  Popup popup = Popup::New();
  popup.SetProperty(Dali::Actor::Property::NAME, "ResetPopup");
  popup.SetStyleName("ResetPopup");
  popup.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
  popup.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
  popup.SetProperty(Actor::Property::SIZE, Vector2(window.GetSize().GetWidth() * 0.75f, 0.0f));
  popup.SetProperty(Popup::Property::TAIL_VISIBILITY, false);
  popup.OutsideTouchedSignal().Connect(this, &StylingApplication::HidePopup);
  popup.HiddenSignal().Connect(this, &StylingApplication::PopupHidden);

  popup.SetTitle(CreateTitle("Reset channels"));

  TextLabel text = TextLabel::New("This will reset the channel data to full value. Are you sure?");
  text.SetProperty(Dali::Actor::Property::NAME, "PopupContentText");
  text.SetStyleName("PopupBody");
  text.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::WIDTH);
  text.SetResizePolicy(ResizePolicy::DIMENSION_DEPENDENCY, Dimension::HEIGHT);
  text.SetProperty(TextLabel::Property::MULTI_LINE, true);
  text.SetProperty(Actor::Property::PADDING, Padding(10.0f, 10.0f, 20.0f, 0.0f));
  popup.SetContent(text);

  ImageView footer = ImageView::New(DEFAULT_CONTROL_AREA_IMAGE_PATH);
  footer.SetProperty(Dali::Actor::Property::NAME, "PopupFooter");
  footer.SetStyleName("PopupFooter");
  footer.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::WIDTH);
  footer.SetResizePolicy(ResizePolicy::FIXED, Dimension::HEIGHT);
  footer.SetProperty(Actor::Property::SIZE, Vector2(0.0f, 80.0f));
  footer.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
  footer.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);

  TableView footerLayout = TableView::New(1, 2);
  footerLayout.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
  footerLayout.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
  footerLayout.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::ALL_DIMENSIONS);

  PushButton okayButton = PushButton::New();
  okayButton.SetProperty(Dali::Actor::Property::NAME, POPUP_CONTROL_OK_NAME);
  okayButton.SetStyleName(POPUP_CONTROL_OK_NAME);
  okayButton.SetProperty(Toolkit::Button::Property::LABEL, "Ok!");
  okayButton.ClickedSignal().Connect(this, &StylingApplication::OnReset);
  okayButton.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
  okayButton.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
  okayButton.SetResizePolicy(ResizePolicy::SIZE_FIXED_OFFSET_FROM_PARENT, Dimension::ALL_DIMENSIONS);
  okayButton.SetProperty(Actor::Property::SIZE_MODE_FACTOR, Vector3(-20.0f, -20.0f, 0.0f));

  PushButton cancelButton = PushButton::New();
  cancelButton.SetProperty(Dali::Actor::Property::NAME, POPUP_CONTROL_CANCEL_NAME);
  cancelButton.SetStyleName(POPUP_CONTROL_CANCEL_NAME);
  cancelButton.SetProperty(Toolkit::Button::Property::LABEL, "Cancel");
  cancelButton.ClickedSignal().Connect(this, &StylingApplication::OnResetCancelled);
  cancelButton.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
  cancelButton.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
  cancelButton.SetResizePolicy(ResizePolicy::SIZE_FIXED_OFFSET_FROM_PARENT, Dimension::ALL_DIMENSIONS);
  cancelButton.SetProperty(Actor::Property::SIZE_MODE_FACTOR, Vector3(-20.0f, -20.0f, 0.0f));

  footerLayout.SetCellAlignment(TableView::CellPosition(0, 0), HorizontalAlignment::CENTER, VerticalAlignment::CENTER);
  footerLayout.SetCellAlignment(TableView::CellPosition(0, 1), HorizontalAlignment::CENTER, VerticalAlignment::CENTER);
  footerLayout.AddChild(okayButton, TableView::CellPosition(0, 0));
  footerLayout.AddChild(cancelButton, TableView::CellPosition(0, 1));
  footerLayout.SetCellPadding(Size(10.f, 10.f));
  footer.Add(footerLayout);

  popup.SetFooter(footer);
  return popup;
}

TextLabel StylingApplication::CreateTitle(std::string title)
{
  TextLabel titleActor = TextLabel::New(title);
  titleActor.SetProperty(Dali::Actor::Property::NAME, "titleActor");
  titleActor.SetStyleName("PopupTitle");
  titleActor.SetProperty(TextLabel::Property::HORIZONTAL_ALIGNMENT, "CENTER");
  titleActor.SetProperty(TextLabel::Property::MULTI_LINE, false);

  return titleActor;
}

bool StylingApplication::OnButtonStateChange(Button button)
{
  // On button press, called twice, once to tell new button it's selected,
  // once to tell old button it isn't selected?

  // Todo: save / restore slider states per image

  if(button.GetProperty<bool>(Button::Property::SELECTED))
  {
    ImageChannelControl prevIcc = mImageChannelControl;

    if(mRadioButtons[0].GetProperty<bool>(Button::Property::SELECTED))
    {
      mImageChannelControl = mIcc1;
    }
    else if(mRadioButtons[1].GetProperty<bool>(Button::Property::SELECTED))
    {
      mImageChannelControl = mIcc2;
    }
    else if(mRadioButtons[2].GetProperty<bool>(Button::Property::SELECTED))
    {
      mImageChannelControl = mIcc3;
    }

    if(prevIcc)
    {
      prevIcc.SetVisibility(false);
    }

    if(mImageChannelControl)
    {
      mImageChannelControl.SetVisibility(true);
    }
  }

  return true;
}

bool StylingApplication::OnCheckButtonChange(Button button)
{
  Property::Index index = button.GetPropertyIndex("channel");
  if(index != Property::INVALID_INDEX)
  {
    int   channel = button["channel"];
    float value   = mChannelSliders[channel][Slider::Property::VALUE];
    if(!button[Button::Property::SELECTED])
    {
      // "Turn off" the channel's contribution
      value = 0.0f;
    }
    Property::Index channelIndex = GetChannelProperty(channel);
    mImageChannelControl.SetProperty(channelIndex, value / 100.0f);
  }
  return true;
}

bool StylingApplication::OnThemeButtonClicked(Button button)
{
  int         theme     = button.GetProperty<int>(button.GetPropertyIndex("theme"));
  const char* themePath = NULL;

  switch(theme)
  {
    case 0:
    {
      themePath = DEMO_THEME_ONE_PATH;
      break;
    }
    case 1:
    {
      themePath = DEMO_THEME_TWO_PATH;
      break;
    }
    case 2:
    {
      themePath = DEMO_THEME_THREE_PATH;
      break;
    }
  }

  StyleManager::Get().ApplyTheme(themePath);

  return true;
}

bool StylingApplication::OnResetClicked(Button button)
{
  if(!mResetPopup)
  {
    mResetPopup = CreateResetPopup();
  }

  mApplication.GetWindow().Add(mResetPopup);

  mResetPopup.SetDisplayState(Popup::SHOWN);
  return true;
}

bool StylingApplication::OnReset(Button button)
{
  // todo: Reset the sliders for this image
  for(int i = 0; i < 3; ++i)
  {
    mChannelSliders[i].SetProperty(Slider::Property::VALUE, 100.0f);
  }
  HidePopup();
  return true;
}

bool StylingApplication::OnResetCancelled(Button button)
{
  HidePopup();
  return true;
}

bool StylingApplication::OnSliderChanged(Slider slider, float value)
{
  // todo - change color channel's saturation
  Property::Index index = slider.GetPropertyIndex("channel");
  if(index != Property::INVALID_INDEX)
  {
    int channel = slider["channel"];
    if(mCheckButtons[channel]["selected"])
    {
      Property::Index channelIndex = GetChannelProperty(channel);
      mImageChannelControl.SetProperty(channelIndex, value / 100.0f);
    }
  }
  return true;
}

void StylingApplication::HidePopup()
{
  if(mResetPopup)
  {
    mResetPopup.SetDisplayState(Popup::HIDDEN);
  }
}

void StylingApplication::PopupHidden()
{
  if(mResetPopup)
  {
    // Clear down resources
    mResetPopup.Unparent();
    mResetPopup.Reset();
  }
}

void StylingApplication::OnPan(Actor actor, const PanGesture& gesture)
{
  Vector3 size = mContentPane.GetTargetSize();
  mContentPane.SetProperty(Actor::Property::SIZE, Vector2(size.GetVectorXY() + gesture.GetDisplacement() * 2.0f));
}

void StylingApplication::OnKeyEvent(const KeyEvent& keyEvent)
{
  static int keyPressed = 0;

  if(keyEvent.GetState() == KeyEvent::DOWN)
  {
    if(keyPressed == 0) // Is this the first down event?
    {
      printf("Key pressed: %s %d\n", keyEvent.GetKeyName().c_str(), keyEvent.GetKeyCode());

      if(IsKey(keyEvent, DALI_KEY_ESCAPE) || IsKey(keyEvent, DALI_KEY_BACK))
      {
        mApplication.Quit();
      }
      else if(keyEvent.GetKeyName().compare("Return") == 0)
      {
        mCurrentTheme++;
        mCurrentTheme %= NUMBER_OF_THEMES;

        StyleManager styleManager = StyleManager::Get();
        switch(mCurrentTheme)
        {
          case 0:
          {
            styleManager.ApplyTheme(DEMO_THEME_ONE_PATH);
            printf("Changing to theme One\n");
            break;
          }
          case 1:
          {
            styleManager.ApplyTheme(DEMO_THEME_TWO_PATH);
            printf("Changing to theme Two\n");
            break;
          }
          case 2:
          {
            styleManager.ApplyDefaultTheme();
            printf("Changing to Toolkit theme\n");
            break;
          }
        }
      }
    }
    keyPressed = 1;
  }
  else if(keyEvent.GetState() == KeyEvent::UP)
  {
    keyPressed = 0;
  }
}

} // namespace Demo
