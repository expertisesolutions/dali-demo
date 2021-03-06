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
 *
 */

#include <sstream>
#include "shared/view.h"

#include <dali-toolkit/dali-toolkit.h>
#include <dali-toolkit/devel-api/visuals/visual-properties-devel.h>
#include <dali/dali.h>

using namespace Dali;
using namespace Dali::Toolkit;

namespace
{
enum AllImagesLayouts
{
  SPIRAL_LAYOUT,
  DEPTH_LAYOUT,
  GRID_LAYOUT,

  NUMBER_OF_LAYOUTS
};

const char* IMAGE_PATHS[] = {
  DEMO_IMAGE_DIR "gallery-medium-1.jpg",
  DEMO_IMAGE_DIR "gallery-medium-2.jpg",
  DEMO_IMAGE_DIR "gallery-medium-3.jpg",
  DEMO_IMAGE_DIR "gallery-medium-4.jpg",
  DEMO_IMAGE_DIR "gallery-medium-5.jpg",
  DEMO_IMAGE_DIR "gallery-medium-6.jpg",
  DEMO_IMAGE_DIR "gallery-medium-7.jpg",
  DEMO_IMAGE_DIR "gallery-medium-8.jpg",
  DEMO_IMAGE_DIR "gallery-medium-9.jpg",
  DEMO_IMAGE_DIR "gallery-medium-10.jpg",
  DEMO_IMAGE_DIR "gallery-medium-11.jpg",
  DEMO_IMAGE_DIR "gallery-medium-12.jpg",
  DEMO_IMAGE_DIR "gallery-medium-13.jpg",
  DEMO_IMAGE_DIR "gallery-medium-14.jpg",
  DEMO_IMAGE_DIR "gallery-medium-15.jpg",
  DEMO_IMAGE_DIR "gallery-medium-16.jpg",
  DEMO_IMAGE_DIR "gallery-medium-17.jpg",
  DEMO_IMAGE_DIR "gallery-medium-18.jpg",
  DEMO_IMAGE_DIR "gallery-medium-19.jpg",
  DEMO_IMAGE_DIR "gallery-medium-20.jpg",
  DEMO_IMAGE_DIR "gallery-medium-21.jpg",
  DEMO_IMAGE_DIR "gallery-medium-22.jpg",
  DEMO_IMAGE_DIR "gallery-medium-23.jpg",
  DEMO_IMAGE_DIR "gallery-medium-24.jpg",
  DEMO_IMAGE_DIR "gallery-medium-25.jpg",
  DEMO_IMAGE_DIR "gallery-medium-26.jpg",
  DEMO_IMAGE_DIR "gallery-medium-27.jpg",
  DEMO_IMAGE_DIR "gallery-medium-28.jpg",
  DEMO_IMAGE_DIR "gallery-medium-29.jpg",
  DEMO_IMAGE_DIR "gallery-medium-30.jpg",
  DEMO_IMAGE_DIR "gallery-medium-31.jpg",
  DEMO_IMAGE_DIR "gallery-medium-32.jpg",
  DEMO_IMAGE_DIR "gallery-medium-33.jpg",
  DEMO_IMAGE_DIR "gallery-medium-34.jpg",
  DEMO_IMAGE_DIR "gallery-medium-35.jpg",
  DEMO_IMAGE_DIR "gallery-medium-36.jpg",
  DEMO_IMAGE_DIR "gallery-medium-37.jpg",
  DEMO_IMAGE_DIR "gallery-medium-38.jpg",
  DEMO_IMAGE_DIR "gallery-medium-39.jpg",
  DEMO_IMAGE_DIR "gallery-medium-40.jpg",
  DEMO_IMAGE_DIR "gallery-medium-41.jpg",
  DEMO_IMAGE_DIR "gallery-medium-42.jpg",
  DEMO_IMAGE_DIR "gallery-medium-43.jpg",
  DEMO_IMAGE_DIR "gallery-medium-44.jpg",
  DEMO_IMAGE_DIR "gallery-medium-45.jpg",
  DEMO_IMAGE_DIR "gallery-medium-46.jpg",
  DEMO_IMAGE_DIR "gallery-medium-47.jpg",
  DEMO_IMAGE_DIR "gallery-medium-48.jpg",
  DEMO_IMAGE_DIR "gallery-medium-49.jpg",
  DEMO_IMAGE_DIR "gallery-medium-50.jpg",
  DEMO_IMAGE_DIR "gallery-medium-51.jpg",
  DEMO_IMAGE_DIR "gallery-medium-52.jpg",
  DEMO_IMAGE_DIR "gallery-medium-53.jpg",
};

const unsigned int NUM_IMAGES = sizeof(IMAGE_PATHS) / sizeof(char*);

const char* BACKGROUND_IMAGE("");
const char* TOOLBAR_IMAGE(DEMO_IMAGE_DIR "top-bar.png");
const char* EDIT_IMAGE(DEMO_IMAGE_DIR "icon-edit.png");
const char* EDIT_IMAGE_SELECTED(DEMO_IMAGE_DIR "icon-edit-selected.png");
const char* SPIRAL_LAYOUT_IMAGE(DEMO_IMAGE_DIR "icon-item-view-layout-spiral.png");
const char* SPIRAL_LAYOUT_IMAGE_SELECTED(DEMO_IMAGE_DIR "icon-item-view-layout-spiral-selected.png");
const char* GRID_LAYOUT_IMAGE(DEMO_IMAGE_DIR "icon-item-view-layout-grid.png");
const char* GRID_LAYOUT_IMAGE_SELECTED(DEMO_IMAGE_DIR "icon-item-view-layout-grid-selected.png");
const char* DEPTH_LAYOUT_IMAGE(DEMO_IMAGE_DIR "icon-item-view-layout-depth.png");
const char* DEPTH_LAYOUT_IMAGE_SELECTED(DEMO_IMAGE_DIR "icon-item-view-layout-depth-selected.png");
const char* DELETE_IMAGE(DEMO_IMAGE_DIR "icon-delete.png");
const char* DELETE_IMAGE_SELECTED(DEMO_IMAGE_DIR "icon-delete-selected.png");
const char* REPLACE_IMAGE(DEMO_IMAGE_DIR "icon-replace.png");
const char* REPLACE_IMAGE_SELECTED(DEMO_IMAGE_DIR "icon-replace-selected.png");
const char* INSERT_IMAGE(DEMO_IMAGE_DIR "icon-insert.png");
const char* INSERT_IMAGE_SELECTED(DEMO_IMAGE_DIR "icon-insert-selected.png");
const char* SELECTED_IMAGE(DEMO_IMAGE_DIR "item-select-check.png");
const char* APPLICATION_TITLE("ItemView");

const char* SPIRAL_LABEL("Spiral");
const char* GRID_LABEL("Grid");
const char* DEPTH_LABEL("Depth");

const float ITEM_BORDER_SIZE = 2.0f;

const float DEPTH_LAYOUT_ITEM_SIZE_FACTOR_PORTRAIT  = 1.0f;
const float DEPTH_LAYOUT_ITEM_SIZE_FACTOR_LANDSCAPE = 0.8f;
const float DEPTH_LAYOUT_COLUMNS                    = 3.0f;

const float MIN_SWIPE_DISTANCE = 15.0f;
const float MIN_SWIPE_SPEED    = 5.0f;

const float SELECTION_BORDER_WIDTH = 3.0f;
const float BUTTON_BORDER          = -10.0f;
const float MENU_OPTION_HEIGHT(140.0f);
const float LABEL_TEXT_SIZE_Y = 20.0f;

const Vector3 INITIAL_OFFSCREEN_POSITION(1000.0f, 0, -1000.0f);

const float SCROLL_TO_ITEM_ANIMATION_TIME = 5.f;

static Vector3 DepthLayoutItemSizeFunctionPortrait(float layoutWidth)
{
  float width = (layoutWidth / (DEPTH_LAYOUT_COLUMNS + 1.0f)) * DEPTH_LAYOUT_ITEM_SIZE_FACTOR_PORTRAIT;

  // 1x1 aspect ratio
  return Vector3(width, width, width);
}

static Vector3 DepthLayoutItemSizeFunctionLandscape(float layoutWidth)
{
  float width = (layoutWidth / (DEPTH_LAYOUT_COLUMNS + 1.0f)) * DEPTH_LAYOUT_ITEM_SIZE_FACTOR_LANDSCAPE;

  // 1x1 aspect ratio
  return Vector3(width, width, width);
}

} // unnamed namespace

/**
 * This example shows how to use ItemView UI control.
 * There are three layouts created for ItemView, i.e., Spiral, Depth and Grid.
 * There is one button in the upper-left corner for quitting the application and
 * another button in the upper-right corner for switching between different layouts.
 */
class ItemViewExample : public ConnectionTracker, public ItemFactory
{
public:
  enum Mode
  {
    MODE_NORMAL,
    MODE_REMOVE,
    MODE_REMOVE_MANY,
    MODE_INSERT,
    MODE_INSERT_MANY,
    MODE_REPLACE,
    MODE_REPLACE_MANY,
    MODE_LAST
  };

  /**
   * Constructor
   * @param application class, stored as reference
   */
  ItemViewExample(Application& application)
  : mApplication(application),
    mMode(MODE_NORMAL),
    mOrientation(0),
    mCurrentLayout(SPIRAL_LAYOUT),
    mDurationSeconds(0.25f)
  {
    // Connect to the Application's Init signal
    mApplication.InitSignal().Connect(this, &ItemViewExample::OnInit);
  }

  /**
   * This method gets called once the main loop of application is up and running
   */
  void OnInit(Application& app)
  {
    Window window = app.GetWindow();
    window.KeyEventSignal().Connect(this, &ItemViewExample::OnKeyEvent);
    window.GetRootLayer().SetProperty(Layer::Property::BEHAVIOR, Layer::LAYER_3D);

    Vector2 windowSize = window.GetSize();

    // Creates a default view with a default tool bar.
    // The view is added to the window.

    Layer contents = DemoHelper::CreateView(mApplication,
                                            mView,
                                            mToolBar,
                                            BACKGROUND_IMAGE,
                                            TOOLBAR_IMAGE,
                                            "");

    // Create an edit mode button. (left of toolbar)
    Toolkit::PushButton editButton = Toolkit::PushButton::New();
    editButton.SetProperty(Toolkit::Button::Property::UNSELECTED_BACKGROUND_VISUAL, EDIT_IMAGE);
    editButton.SetProperty(Toolkit::Button::Property::SELECTED_BACKGROUND_VISUAL, EDIT_IMAGE_SELECTED);
    editButton.ClickedSignal().Connect(this, &ItemViewExample::OnModeButtonClicked);
    editButton.SetProperty(Actor::Property::LEAVE_REQUIRED, true);
    mToolBar.AddControl(editButton, DemoHelper::DEFAULT_VIEW_STYLE.mToolBarButtonPercentage, Toolkit::Alignment::HORIZONTAL_LEFT, DemoHelper::DEFAULT_MODE_SWITCH_PADDING);

    // Create a layout toggle button. (right of toolbar)
    mLayoutButton = Toolkit::PushButton::New();
    mLayoutButton.SetProperty(Toolkit::Button::Property::UNSELECTED_BACKGROUND_VISUAL, SPIRAL_LAYOUT_IMAGE);
    mLayoutButton.SetProperty(Toolkit::Button::Property::SELECTED_BACKGROUND_VISUAL, SPIRAL_LAYOUT_IMAGE_SELECTED);
    mLayoutButton.ClickedSignal().Connect(this, &ItemViewExample::OnLayoutButtonClicked);
    mLayoutButton.SetProperty(Actor::Property::LEAVE_REQUIRED, true);
    mToolBar.AddControl(mLayoutButton, DemoHelper::DEFAULT_VIEW_STYLE.mToolBarButtonPercentage, Toolkit::Alignment::HORIZONTAL_RIGHT, DemoHelper::DEFAULT_MODE_SWITCH_PADDING);

    // Create a delete button (bottom right of screen)
    mDeleteButton = Toolkit::PushButton::New();
    mDeleteButton.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::BOTTOM_RIGHT);
    mDeleteButton.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::BOTTOM_RIGHT);
    mDeleteButton.SetProperty(Actor::Property::POSITION, Vector2(BUTTON_BORDER, BUTTON_BORDER));
    mDeleteButton.SetProperty(Actor::Property::DRAW_MODE, DrawMode::OVERLAY_2D);
    mDeleteButton.SetProperty(Toolkit::Button::Property::UNSELECTED_BACKGROUND_VISUAL, DELETE_IMAGE);
    mDeleteButton.SetProperty(Toolkit::Button::Property::SELECTED_BACKGROUND_VISUAL, DELETE_IMAGE_SELECTED);
    mDeleteButton.SetProperty(Toolkit::Control::Property::BACKGROUND, TOOLBAR_IMAGE);
    mDeleteButton.SetProperty(Actor::Property::SIZE, Vector2(windowSize.width * 0.15f, windowSize.width * 0.15f));
    mDeleteButton.ClickedSignal().Connect(this, &ItemViewExample::OnDeleteButtonClicked);
    mDeleteButton.SetProperty(Actor::Property::LEAVE_REQUIRED, true);
    mDeleteButton.SetProperty(Actor::Property::VISIBLE, false);
    window.Add(mDeleteButton);

    // Create an insert button (bottom right of screen)
    mInsertButton = Toolkit::PushButton::New();
    mInsertButton.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::BOTTOM_RIGHT);
    mInsertButton.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::BOTTOM_RIGHT);
    mInsertButton.SetProperty(Actor::Property::POSITION, Vector2(BUTTON_BORDER, BUTTON_BORDER));
    mInsertButton.SetProperty(Actor::Property::DRAW_MODE, DrawMode::OVERLAY_2D);
    mInsertButton.SetProperty(Toolkit::Button::Property::UNSELECTED_BACKGROUND_VISUAL, INSERT_IMAGE);
    mInsertButton.SetProperty(Toolkit::Button::Property::SELECTED_BACKGROUND_VISUAL, INSERT_IMAGE_SELECTED);
    mInsertButton.SetProperty(Toolkit::Control::Property::BACKGROUND, TOOLBAR_IMAGE);
    mInsertButton.SetProperty(Actor::Property::SIZE, Vector2(windowSize.width * 0.15f, windowSize.width * 0.15f));
    mInsertButton.ClickedSignal().Connect(this, &ItemViewExample::OnInsertButtonClicked);
    mInsertButton.SetProperty(Actor::Property::LEAVE_REQUIRED, true);
    mInsertButton.SetProperty(Actor::Property::VISIBLE, false);
    window.Add(mInsertButton);

    // Create an replace button (bottom right of screen)
    mReplaceButton = Toolkit::PushButton::New();
    mReplaceButton.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::BOTTOM_RIGHT);
    mReplaceButton.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::BOTTOM_RIGHT);
    mReplaceButton.SetProperty(Actor::Property::POSITION, Vector2(BUTTON_BORDER, BUTTON_BORDER));
    mReplaceButton.SetProperty(Actor::Property::DRAW_MODE, DrawMode::OVERLAY_2D);
    mReplaceButton.SetProperty(Toolkit::Button::Property::UNSELECTED_BACKGROUND_VISUAL, REPLACE_IMAGE);
    mReplaceButton.SetProperty(Toolkit::Button::Property::SELECTED_BACKGROUND_VISUAL, REPLACE_IMAGE_SELECTED);
    mReplaceButton.SetProperty(Toolkit::Control::Property::BACKGROUND, TOOLBAR_IMAGE);
    mReplaceButton.SetProperty(Actor::Property::SIZE, Vector2(windowSize.width * 0.15f, windowSize.width * 0.15f));
    mReplaceButton.ClickedSignal().Connect(this, &ItemViewExample::OnReplaceButtonClicked);
    mReplaceButton.SetProperty(Actor::Property::LEAVE_REQUIRED, true);
    mReplaceButton.SetProperty(Actor::Property::VISIBLE, false);
    window.Add(mReplaceButton);

    // Create the item view actor
    mItemView = ItemView::New(*this);
    mItemView.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
    mItemView.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);

    // Display item view on the window
    window.Add(mItemView);
    window.GetRootLayer().SetProperty(Layer::Property::BEHAVIOR, Layer::LAYER_3D);

    // Create the layouts
    mSpiralLayout = DefaultItemLayout::New(DefaultItemLayout::SPIRAL);
    mDepthLayout  = DefaultItemLayout::New(DefaultItemLayout::DEPTH);
    mGridLayout   = DefaultItemLayout::New(DefaultItemLayout::GRID);

    // Add the layouts to item view
    mItemView.AddLayout(*mSpiralLayout);
    mItemView.AddLayout(*mDepthLayout);
    mItemView.AddLayout(*mGridLayout);

    mItemView.SetMinimumSwipeDistance(MIN_SWIPE_DISTANCE);
    mItemView.SetMinimumSwipeSpeed(MIN_SWIPE_SPEED);

    // Activate the spiral layout
    SetLayout(mCurrentLayout);
    mItemView.SetProperty(Actor::Property::KEYBOARD_FOCUSABLE, true);
    KeyboardFocusManager::Get().PreFocusChangeSignal().Connect(this, &ItemViewExample::OnKeyboardPreFocusChange);

    // Set the title and icon to the current layout
    SetLayoutTitle();
    SetLayoutImage();

    mLongPressDetector = LongPressGestureDetector::New();
    mLongPressDetector.Attach(mItemView);
    mLongPressDetector.DetectedSignal().Connect(this, &ItemViewExample::OnLongPress);
  }

  Actor OnKeyboardPreFocusChange(Actor current, Actor proposed, Control::KeyboardFocus::Direction direction)
  {
    if(!current && !proposed)
    {
      return mItemView;
    }

    return proposed;
  }

  /**
   * Animate to a different layout
   */
  void ChangeLayout()
  {
    Animation animation = Animation::New(mDurationSeconds);
    animation.FinishedSignal().Connect(this, &ItemViewExample::AnimationFinished);
    animation.AnimateTo(Property(mItemView, Actor::Property::COLOR_ALPHA), 0.0f);
    animation.Play();
  }

  void AnimationFinished(Animation&)
  {
    SetLayout(mCurrentLayout);

    Animation animation = Animation::New(mDurationSeconds);
    animation.AnimateTo(Property(mItemView, Actor::Property::COLOR_ALPHA), 1.0f);
    animation.Play();
  }

  /**
   * Switch to a different item view layout
   */
  void SetLayout(int layoutId)
  {
    Window window = mApplication.GetWindow();
    switch(mCurrentLayout)
    {
      case SPIRAL_LAYOUT:
      case DEPTH_LAYOUT:
      {
        window.GetRootLayer().SetProperty(Layer::Property::BEHAVIOR, Layer::LAYER_3D);
        break;
      }
      case GRID_LAYOUT:
      {
        window.GetRootLayer().SetProperty(Layer::Property::BEHAVIOR, Layer::LAYER_UI);
        break;
      }
    }

    // Set the new orientation to the layout
    mItemView.GetLayout(layoutId)->SetOrientation(static_cast<ControlOrientation::Type>(mOrientation / 90));

    Vector2 windowSize = window.GetSize();

    if(layoutId == DEPTH_LAYOUT)
    {
      // Set up the depth layout according to the new orientation
      if(Toolkit::IsVertical(mDepthLayout->GetOrientation()))
      {
        mDepthLayout->SetItemSize(DepthLayoutItemSizeFunctionPortrait(windowSize.width));
      }
      else
      {
        mDepthLayout->SetItemSize(DepthLayoutItemSizeFunctionLandscape(windowSize.height));
      }
    }

    // Enable anchoring for depth layout only
    mItemView.SetAnchoring(layoutId == DEPTH_LAYOUT);

    // Activate the layout
    mItemView.ActivateLayout(layoutId, Vector3(windowSize.x, windowSize.y, windowSize.x), 0.0f);
  }

  bool OnLayoutButtonClicked(Toolkit::Button button)
  {
    // Switch to the next layout
    mCurrentLayout = (mCurrentLayout + 1) % mItemView.GetLayoutCount();

    ChangeLayout();

    SetLayoutTitle();
    SetLayoutImage();

    return true;
  }

  bool OnModeButtonClicked(Toolkit::Button button)
  {
    SwitchToNextMode();

    return true;
  }

  void SwitchToNextMode()
  {
    switch(mMode)
    {
      case MODE_REMOVE:
      {
        ExitRemoveMode();
        mMode = MODE_REMOVE_MANY;
        EnterRemoveManyMode();
        break;
      }

      case MODE_REMOVE_MANY:
      {
        ExitRemoveManyMode();
        mMode = MODE_INSERT;
        EnterInsertMode();
        break;
      }

      case MODE_INSERT:
      {
        ExitInsertMode();
        mMode = MODE_INSERT_MANY;
        EnterInsertManyMode();
        break;
      }

      case MODE_INSERT_MANY:
      {
        ExitInsertManyMode();
        mMode = MODE_REPLACE;
        EnterReplaceMode();
        break;
      }

      case MODE_REPLACE:
      {
        ExitReplaceMode();
        mMode = MODE_REPLACE_MANY;
        EnterReplaceManyMode();
        break;
      }

      case MODE_REPLACE_MANY:
      {
        ExitReplaceManyMode();
        mMode = MODE_NORMAL;
        SetLayoutTitle();
        break;
      }

      case MODE_NORMAL:
      default:
      {
        mMode = MODE_REMOVE;
        EnterRemoveMode();
        break;
      }
    }
  }

  void EnterRemoveMode()
  {
    SetTitle("Edit: Remove");

    mTapDetector = TapGestureDetector::New();

    for(unsigned int i = 0u; i < mItemView.GetChildCount(); ++i)
    {
      mTapDetector.Attach(mItemView.GetChildAt(i));
    }

    mTapDetector.DetectedSignal().Connect(this, &ItemViewExample::RemoveOnTap);
  }

  void ExitRemoveMode()
  {
    mTapDetector.Reset();
  }

  void RemoveOnTap(Actor actor, const TapGesture& tap)
  {
    mItemView.RemoveItem(mItemView.GetItemId(actor), 0.5f);
  }

  void EnterRemoveManyMode()
  {
    SetTitle("Edit: Remove Many");

    mDeleteButton.SetProperty(Actor::Property::VISIBLE, true);

    mTapDetector = TapGestureDetector::New();

    for(unsigned int i = 0u; i < mItemView.GetChildCount(); ++i)
    {
      Actor child = mItemView.GetChildAt(i);
      Actor box   = child.FindChildByName("CheckBox");

      if(box)
      {
        mTapDetector.Attach(child);
        box.SetProperty(Actor::Property::VISIBLE, true);
      }
    }

    mTapDetector.DetectedSignal().Connect(this, &ItemViewExample::SelectOnTap);
  }

  void ExitRemoveManyMode()
  {
    for(unsigned int i = 0u; i < mItemView.GetChildCount(); ++i)
    {
      Actor child = mItemView.GetChildAt(i);
      Actor box   = child.FindChildByName("CheckBox");

      if(box)
      {
        box.SetProperty(Actor::Property::VISIBLE, false);

        Actor tick = box.FindChildByName("Tick");
        if(tick)
        {
          tick.SetProperty(Actor::Property::VISIBLE, false);
        }
      }
    }

    mTapDetector.Reset();

    mDeleteButton.SetProperty(Actor::Property::VISIBLE, false);
  }

  void SelectOnTap(Actor actor, const TapGesture& tap)
  {
    Actor tick = actor.FindChildByName("Tick");
    if(tick)
    {
      tick.SetProperty(Actor::Property::VISIBLE, !tick.GetCurrentProperty<bool>(Actor::Property::VISIBLE));
    }
  }

  void OnLongPress(Actor actor, const LongPressGesture& gesture)
  {
    switch(gesture.GetState())
    {
      case GestureState::STARTED:
      {
        const Size& size = mApplication.GetWindow().GetSize();

        ItemRange range(0u, 0u);
        mItemView.GetItemsRange(range);

        const unsigned int item = (gesture.GetScreenPoint().y < 0.5f * size.height) ? range.begin : range.end;
        mItemView.ScrollToItem(item, SCROLL_TO_ITEM_ANIMATION_TIME);

        break;
      }
      case GestureState::FINISHED:
      {
        Property::Map attributes;
        mItemView.DoAction("stopScrolling", attributes);
        break;
      }
      default:
      {
        break;
      }
    }
  }

  bool OnDeleteButtonClicked(Toolkit::Button button)
  {
    ItemIdContainer removeList;

    for(unsigned int i = 0u; i < mItemView.GetChildCount(); ++i)
    {
      Actor child = mItemView.GetChildAt(i);
      Actor tick  = child.FindChildByName("Tick");

      if(tick && tick.GetCurrentProperty<bool>(Actor::Property::VISIBLE))
      {
        removeList.push_back(mItemView.GetItemId(child));
      }
    }

    if(!removeList.empty())
    {
      mItemView.RemoveItems(removeList, 0.5f);
    }

    return true;
  }

  void EnterInsertMode()
  {
    SetTitle("Edit: Insert");

    mTapDetector = TapGestureDetector::New();

    for(unsigned int i = 0u; i < mItemView.GetChildCount(); ++i)
    {
      mTapDetector.Attach(mItemView.GetChildAt(i));
    }

    mTapDetector.DetectedSignal().Connect(this, &ItemViewExample::InsertOnTap);
  }

  void ExitInsertMode()
  {
    mTapDetector.Reset();
  }

  void InsertOnTap(Actor actor, const TapGesture& tap)
  {
    ItemId id = mItemView.GetItemId(actor);

    Actor newActor = NewItem(rand());

    mItemView.InsertItem(Item(id, newActor), 0.5f);
  }

  void EnterInsertManyMode()
  {
    SetTitle("Edit: Insert Many");

    mInsertButton.SetProperty(Actor::Property::VISIBLE, true);

    mTapDetector = TapGestureDetector::New();

    for(unsigned int i = 0u; i < mItemView.GetChildCount(); ++i)
    {
      Actor child = mItemView.GetChildAt(i);
      Actor box   = child.FindChildByName("CheckBox");

      if(box)
      {
        mTapDetector.Attach(child);
        box.SetProperty(Actor::Property::VISIBLE, true);
      }
    }

    mTapDetector.DetectedSignal().Connect(this, &ItemViewExample::SelectOnTap);
  }

  void ExitInsertManyMode()
  {
    for(unsigned int i = 0u; i < mItemView.GetChildCount(); ++i)
    {
      Actor child = mItemView.GetChildAt(i);
      Actor box   = child.FindChildByName("CheckBox");

      if(box)
      {
        box.SetProperty(Actor::Property::VISIBLE, false);

        Actor tick = box.FindChildByName("Tick");
        if(tick)
        {
          tick.SetProperty(Actor::Property::VISIBLE, false);
        }
      }
    }

    mTapDetector.Reset();

    mInsertButton.SetProperty(Actor::Property::VISIBLE, false);
  }

  bool OnInsertButtonClicked(Toolkit::Button button)
  {
    ItemContainer insertList;

    for(unsigned int i = 0u; i < mItemView.GetChildCount(); ++i)
    {
      Actor child = mItemView.GetChildAt(i);
      Actor tick  = child.FindChildByName("Tick");

      if(tick && tick.GetCurrentProperty<bool>(Actor::Property::VISIBLE))
      {
        insertList.push_back(Item(mItemView.GetItemId(child), NewItem(rand())));
      }
    }

    if(!insertList.empty())
    {
      mItemView.InsertItems(insertList, 0.5f);
    }

    return true;
  }

  void EnterReplaceMode()
  {
    SetTitle("Edit: Replace");

    mTapDetector = TapGestureDetector::New();

    for(unsigned int i = 0u; i < mItemView.GetChildCount(); ++i)
    {
      mTapDetector.Attach(mItemView.GetChildAt(i));
    }

    mTapDetector.DetectedSignal().Connect(this, &ItemViewExample::ReplaceOnTap);
  }

  void ReplaceOnTap(Actor actor, const TapGesture& tap)
  {
    mItemView.ReplaceItem(Item(mItemView.GetItemId(actor), NewItem(rand())), 0.5f);
  }

  void ExitReplaceMode()
  {
    mTapDetector.Reset();
  }

  void EnterReplaceManyMode()
  {
    SetTitle("Edit: Replace Many");

    mReplaceButton.SetProperty(Actor::Property::VISIBLE, true);

    mTapDetector = TapGestureDetector::New();

    for(unsigned int i = 0u; i < mItemView.GetChildCount(); ++i)
    {
      Actor child = mItemView.GetChildAt(i);
      Actor box   = child.FindChildByName("CheckBox");

      if(box)
      {
        mTapDetector.Attach(child);
        box.SetProperty(Actor::Property::VISIBLE, true);
      }
    }

    mTapDetector.DetectedSignal().Connect(this, &ItemViewExample::SelectOnTap);
  }

  void ExitReplaceManyMode()
  {
    for(unsigned int i = 0u; i < mItemView.GetChildCount(); ++i)
    {
      Actor child = mItemView.GetChildAt(i);
      Actor box   = child.FindChildByName("CheckBox");

      if(box)
      {
        box.SetProperty(Actor::Property::VISIBLE, false);

        Actor tick = box.FindChildByName("Tick");
        if(tick)
        {
          tick.SetProperty(Actor::Property::VISIBLE, false);
        }
      }
    }

    mTapDetector.Reset();

    mReplaceButton.SetProperty(Actor::Property::VISIBLE, false);
  }

  bool OnReplaceButtonClicked(Toolkit::Button button)
  {
    ItemContainer replaceList;

    for(unsigned int i = 0u; i < mItemView.GetChildCount(); ++i)
    {
      Actor child = mItemView.GetChildAt(i);
      Actor tick  = child.FindChildByName("Tick");

      if(tick && tick.GetCurrentProperty<bool>(Actor::Property::VISIBLE))
      {
        replaceList.push_back(Item(mItemView.GetItemId(child), NewItem(rand())));
      }
    }

    if(!replaceList.empty())
    {
      mItemView.ReplaceItems(replaceList, 0.5f);
    }

    return true;
  }

  void SetLayoutTitle()
  {
    if(MODE_NORMAL == mMode)
    {
      std::stringstream ss(APPLICATION_TITLE);
      switch(mCurrentLayout)
      {
        case SPIRAL_LAYOUT:
          ss << APPLICATION_TITLE << ": " << SPIRAL_LABEL;
          break;
        case GRID_LAYOUT:
          ss << APPLICATION_TITLE << ": " << GRID_LABEL;
          break;
        case DEPTH_LAYOUT:
          ss << APPLICATION_TITLE << ": " << DEPTH_LABEL;
          break;
        default:
          break;
      }
      SetTitle(ss.str());
    }
  }

  void SetLayoutImage()
  {
    if(mLayoutButton)
    {
      switch(mCurrentLayout)
      {
        case SPIRAL_LAYOUT:
        {
          mLayoutButton.SetProperty(Toolkit::Button::Property::UNSELECTED_BACKGROUND_VISUAL, SPIRAL_LAYOUT_IMAGE);
          mLayoutButton.SetProperty(Toolkit::Button::Property::SELECTED_BACKGROUND_VISUAL, SPIRAL_LAYOUT_IMAGE_SELECTED);
          break;
        }

        case GRID_LAYOUT:
        {
          mLayoutButton.SetProperty(Toolkit::Button::Property::UNSELECTED_BACKGROUND_VISUAL, GRID_LAYOUT_IMAGE);
          mLayoutButton.SetProperty(Toolkit::Button::Property::SELECTED_BACKGROUND_VISUAL, GRID_LAYOUT_IMAGE_SELECTED);
          break;
        }

        case DEPTH_LAYOUT:
        {
          mLayoutButton.SetProperty(Toolkit::Button::Property::UNSELECTED_BACKGROUND_VISUAL, DEPTH_LAYOUT_IMAGE);
          mLayoutButton.SetProperty(Toolkit::Button::Property::SELECTED_BACKGROUND_VISUAL, DEPTH_LAYOUT_IMAGE_SELECTED);
          break;
        }

        default:
          break;
      }
    }
  }

public: // From ItemFactory
  /**
   * Query the number of items available from the factory.
   * The maximum available item has an ID of GetNumberOfItems() - 1.
   */
  virtual unsigned int GetNumberOfItems()
  {
    return NUM_IMAGES * 10;
  }

  /**
   * Create an Actor to represent a visible item.
   * @param itemId
   * @return the created actor.
   */
  virtual Actor NewItem(unsigned int itemId)
  {
    // Create an image view for this item
    Property::Map propertyMap;
    propertyMap.Insert(Toolkit::Visual::Property::TYPE, Visual::IMAGE);
    propertyMap.Insert(ImageVisual::Property::URL, IMAGE_PATHS[itemId % NUM_IMAGES]);
    propertyMap.Insert(DevelVisual::Property::VISUAL_FITTING_MODE, DevelVisual::FILL);
    ImageView actor = ImageView::New();
    actor.SetProperty(Toolkit::ImageView::Property::IMAGE, propertyMap);
    actor.SetProperty(Actor::Property::POSITION_Z, 0.0f);
    actor.SetProperty(Actor::Property::POSITION, INITIAL_OFFSCREEN_POSITION);

    // Add a border image child actor
    ImageView borderActor = ImageView::New();
    borderActor.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
    borderActor.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
    borderActor.SetResizePolicy(ResizePolicy::SIZE_FIXED_OFFSET_FROM_PARENT, Dimension::ALL_DIMENSIONS);
    borderActor.SetProperty(Actor::Property::SIZE_MODE_FACTOR, Vector3(2.0f * ITEM_BORDER_SIZE, 2.0f * ITEM_BORDER_SIZE, 0.0f));
    borderActor.SetProperty(Actor::Property::COLOR_MODE, USE_PARENT_COLOR);

    Property::Map borderProperty;
    borderProperty.Insert(Toolkit::Visual::Property::TYPE, Visual::BORDER);
    borderProperty.Insert(BorderVisual::Property::COLOR, Color::WHITE);
    borderProperty.Insert(BorderVisual::Property::SIZE, ITEM_BORDER_SIZE);
    borderProperty.Insert(BorderVisual::Property::ANTI_ALIASING, true);
    borderActor.SetProperty(ImageView::Property::IMAGE, borderProperty);

    actor.Add(borderActor);

    actor.SetProperty(Actor::Property::KEYBOARD_FOCUSABLE, true);

    Vector3 spiralItemSize;
    Vector2 windowSize = mApplication.GetWindow().GetSize();
    static_cast<ItemLayout&>(*mSpiralLayout).GetItemSize(0u, Vector3(windowSize), spiralItemSize);

    // Add a checkbox child actor; invisible until edit-mode is enabled
    ImageView checkbox = ImageView::New();
    checkbox.SetProperty(Dali::Actor::Property::NAME, "CheckBox");
    checkbox.SetProperty(Actor::Property::COLOR_MODE, USE_PARENT_COLOR);
    checkbox.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_RIGHT);
    checkbox.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_RIGHT);
    checkbox.SetProperty(Actor::Property::SIZE, Vector2(spiralItemSize.width * 0.2f, spiralItemSize.width * 0.2f));
    checkbox.SetProperty(Actor::Property::POSITION, Vector2(-SELECTION_BORDER_WIDTH, SELECTION_BORDER_WIDTH));
    checkbox.SetProperty(Actor::Property::POSITION_Z, 0.1f);

    Property::Map solidColorProperty;
    solidColorProperty.Insert(Toolkit::Visual::Property::TYPE, Visual::COLOR);
    solidColorProperty.Insert(ColorVisual::Property::MIX_COLOR, Vector4(0.f, 0.f, 0.f, 0.6f));
    checkbox.SetProperty(ImageView::Property::IMAGE, solidColorProperty);

    if(MODE_REMOVE_MANY != mMode &&
       MODE_INSERT_MANY != mMode &&
       MODE_REPLACE_MANY != mMode)
    {
      checkbox.SetProperty(Actor::Property::VISIBLE, false);
    }
    borderActor.Add(checkbox);

    ImageView tick = ImageView::New(SELECTED_IMAGE);
    tick.SetProperty(Dali::Actor::Property::NAME, "Tick");
    tick.SetProperty(Actor::Property::COLOR_MODE, USE_PARENT_COLOR);
    tick.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::TOP_RIGHT);
    tick.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::TOP_RIGHT);
    tick.SetProperty(Actor::Property::SIZE, Vector2(spiralItemSize.width * 0.2f, spiralItemSize.width * 0.2f));
    tick.SetProperty(Actor::Property::POSITION_Z, 0.2f);
    tick.SetProperty(Actor::Property::VISIBLE, false);
    checkbox.Add(tick);

    // Connect new items for various editing modes
    if(mTapDetector)
    {
      mTapDetector.Attach(actor);
    }

    return actor;
  }

private:
  /**
   * Sets/Updates the title of the View
   * @param[in] title The new title for the view.
   */
  void SetTitle(const std::string& title)
  {
    if(!mTitleActor)
    {
      mTitleActor = DemoHelper::CreateToolBarLabel("");
      // Add title to the tool bar.
      mToolBar.AddControl(mTitleActor, DemoHelper::DEFAULT_VIEW_STYLE.mToolBarTitlePercentage, Alignment::HORIZONTAL_CENTER);
    }

    mTitleActor.SetProperty(TextLabel::Property::TEXT, title);
  }

  /**
   * Main key event handler
   */
  void OnKeyEvent(const KeyEvent& event)
  {
    if(event.GetState() == KeyEvent::DOWN)
    {
      if(IsKey(event, DALI_KEY_ESCAPE) || IsKey(event, DALI_KEY_BACK))
      {
        mApplication.Quit();
      }
    }
  }

private:
  Application& mApplication;
  Mode         mMode;

  Toolkit::Control mView;
  unsigned int     mOrientation;

  Toolkit::ToolBar mToolBar;
  TextLabel        mTitleActor; ///< The Toolbar's Title.

  ItemView     mItemView;
  unsigned int mCurrentLayout;
  float        mDurationSeconds;

  ItemLayoutPtr mSpiralLayout;
  ItemLayoutPtr mDepthLayout;
  ItemLayoutPtr mGridLayout;

  TapGestureDetector  mTapDetector;
  Toolkit::PushButton mLayoutButton;
  Toolkit::PushButton mDeleteButton;
  Toolkit::PushButton mInsertButton;
  Toolkit::PushButton mReplaceButton;

  LongPressGestureDetector mLongPressDetector;
};

int DALI_EXPORT_API main(int argc, char** argv)
{
  Application     app = Application::New(&argc, &argv, DEMO_THEME_PATH);
  ItemViewExample test(app);
  app.MainLoop();
  return 0;
}
