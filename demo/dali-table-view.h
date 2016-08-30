#ifndef DALI_DEMO_H
#define DALI_DEMO_H

/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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

#include <dali/dali.h>
#include <dali-toolkit/dali-toolkit.h>
#include <dali-toolkit/devel-api/controls/popup/popup.h>

class Example;

typedef std::vector<Example> ExampleList;
typedef ExampleList::iterator ExampleListIter;
typedef ExampleList::const_iterator ExampleListConstIter;

typedef std::vector<Dali::Animation> AnimationList;
typedef AnimationList::iterator AnimationListIter;
typedef AnimationList::const_iterator AnimationListConstIter;


/**
 * Example information
 *
 * Represents a single Example.
 */
struct Example
{
  // Constructors

  /**
   * @param[in] name unique name of example
   * @param[in] title The caption for the example to appear on a tile button.
   */
  Example(std::string name, std::string title)
  : name(name),
    title(title)
  {
  }

  Example()
  {
  }

  // Data

  std::string name;                       ///< unique name of example
  std::string title;                      ///< title (caption) of example to appear on tile button.
};



/**
 * Dali-Demo instance
 */
class DaliTableView : public Dali::ConnectionTracker
{
public:

  DaliTableView(Dali::Application& application);
  ~DaliTableView();

public:

  /**
   * Adds an Example to our demo showcase
   *
   * @param[in] example The Example description.
   *
   * @note Should be called before the Application MainLoop is started.
   */
  void AddExample(Example example);

  /**
   * Sorts the example list alphabetically by Title if parameter is true.
   *
   * @param[in] sortAlphabetically If true, example list is sorted alphabetically.
   *
   * @note Should be called before the Application MainLoop is started.
   * @note By default the examples are NOT sorted alphabetically by Title.
   */
  void SortAlphabetically( bool sortAlphabetically );

private: // Application callbacks & implementation

  /**
   * Initialize application.
   *
   * @param[in] app Application instance
   */
  void Initialize( Dali::Application& app );

  /**
   * Populates the contents (ScrollView) with all the
   * Examples that have been Added using the AddExample(...)
   * call
   */
  void Populate();

  /**
   * Rotate callback from the device.
   *
   * @param[in] orientation that device notified.
   */
  void OrientationChanged( Dali::Orientation orientation );

  /**
   * Rotates RootActor orientation to that specified.
   *
   * @param[in] degrees The requested angle.
   */
  void Rotate( unsigned int degrees );

  /**
   * Creates a tile for the main menu and toolbar.
   *
   * @param[in] name The unique name for this Tile
   * @param[in] title The text caption that appears on the Tile
   * @param[in] parentSize Tile's parent size.
   * @param[in] color The color (including alpha) of the tiles contents.
   *
   * @return The Actor for the created tile.
   */
  Dali::Actor CreateTile( const std::string& name, const std::string& title, const Dali::Vector3& sizeMultiplier, const Dali::Vector4& color );

  // Signal handlers

  /**
   * Signal emitted when any tile has been pressed
   *
   * @param[in] actor The Actor representing this tile.
   * @param[in] event The Touch information.
   *
   * @return Consume flag
   */
  bool OnTilePressed( Dali::Actor actor, const Dali::TouchData& event );

  /**
   * Called by OnTilePressed & Accessibility to do the appropriate action.
   *
   * @param[in] actor The Actor representing this tile.
   * @param[in] state The Touch state
   *
   * @return Consume flag
   */
  bool DoTilePress( Dali::Actor actor, Dali::PointState::Type state );

  /**
   * Signal emitted when any tile has been hovered
   *
   * @param[in] actor The Actor representing this tile.
   * @param[in] event The HoverEvent
   *
   * @return Consume flag
   */
  bool OnTileHovered( Dali::Actor actor, const Dali::HoverEvent& event );

  /**
   * Signal emitted when the pressed animation has completed.
   *
   * @param[in] source The animation source.
   */
  void OnPressedAnimationFinished(Dali::Animation& source);

  /**
   * Signal emitted when the button has been clicked
   *
   * @param[in] button The Button that is clicked.
   *
   * @return Consume flag
   */
  bool OnButtonClicked( Dali::Toolkit::Button& button );

  /**
   * Signal emitted when scrolling has started.
   *
   * @param[in] position The current position of the scroll contents.
   */
  void OnScrollStart(const Dali::Vector2& position);

  /**
   * Signal emitted when scrolling has completed.
   *
   * @param[in] position The current position of the scroll contents.
   */
  void OnScrollComplete(const Dali::Vector2& position);

  /**
   * Signal emitted when any Sensitive Actor has been touched
   * (other than those touches consumed by OnTilePressed)
   *
   * @param[in] actor The Actor touched.
   * @param[in] event The Touch information.
   *
   * @return Consume flag
   */
  bool OnScrollTouched( Dali::Actor actor, const Dali::TouchData& event );

  /**
   * Setup the effect on the scroll view
   */
  void ApplyScrollViewEffect();

  /**
   * Apply the cube effect to all the page actors
   */
  void ApplyCubeEffectToPages();

  /**
   * Setup the inner cube effect
   */
  void SetupInnerPageCubeEffect();

  /**
   * Apply a shader effect to a table tile
   */
  void ApplyEffectToTile(Dali::Actor tile);

  /**
   * Apply effect to the content of a tile
   */
  void ApplyEffectToTileContent(Dali::Actor tileContent);

  /**
   * Key event handler
   */
  void OnKeyEvent( const Dali::KeyEvent& event );

  /**
   * Creates the logo.
   *
   * @param[in] imagePath The path to the image file to load
   *
   * @return The created image actor
   */
  Dali::Toolkit::ImageView CreateLogo( std::string imagePath );

  /**
   * Callback when the keyboard focus is going to be changed.
   *
   * @param[in] current The current focused actor
   * @param[in] proposed The actor proposed by the keyboard focus manager to move the focus to
   * @param[in] direction The direction to move the focus
   * @return The actor to move the keyboard focus to.
   */
  Dali::Actor OnKeyboardPreFocusChange( Dali::Actor current, Dali::Actor proposed, Dali::Toolkit::Control::KeyboardFocus::Direction direction );

  /**
   * Callback when the keyboard focused actor is activated.
   *
   * @param[in] activatedActor The activated actor
   */
  void OnFocusedActorActivated( Dali::Actor activatedActor );

  /**
   * Called when the logo is tapped
   *
   * @param[in]  actor  The tapped actor
   * @param[in]  tap    The tap information.
   */
  void OnLogoTapped( Dali::Actor actor, const Dali::TapGesture& tap );

  /**
   * Hides the popup
   */
  void HideVersionPopup();

 /*
  * @brief Callback called when the buttons page actor is relaid out
  *
  * @param[in] actor The page actor
  */
 void OnButtonsPageRelayout( const Dali::Actor& actor );

private:

  Dali::Application&              mApplication;              ///< Application instance.
  Dali::Layer                     mBackgroundLayer;          ///< Background resides on a separate layer.
  Dali::Toolkit::TableView        mRootActor;                ///< All content (excluding background is anchored to this Actor)
  Dali::Animation                 mRotateAnimation;          ///< Animation to rotate and resize mRootActor.
  Dali::Animation                 mPressedAnimation;         ///< Button press scaling animation.
  Dali::Layer                     mScrollViewLayer;          ///< ScrollView resides on a separate layer.
  Dali::Toolkit::ScrollView       mScrollView;               ///< ScrollView container (for all Examples)
  Dali::Toolkit::ScrollViewEffect mScrollViewEffect;         ///< Effect to be applied to the scroll view
  Dali::Toolkit::RulerPtr         mScrollRulerX;             ///< ScrollView X (horizontal) ruler
  Dali::Toolkit::RulerPtr         mScrollRulerY;             ///< ScrollView Y (vertical) ruler
  Dali::Actor                     mPressedActor;             ///< The currently pressed actor.
  Dali::TapGestureDetector        mLogoTapDetector;          ///< To detect taps on the logo
  Dali::Toolkit::Popup            mVersionPopup;             ///< Displays DALi library version information

  std::vector< Dali::Actor >      mPages;                    ///< List of pages.
  ExampleList                     mExampleList;              ///< List of examples.

  int                             mTotalPages;               ///< Total pages within scrollview.

  bool                            mScrolling:1;              ///< Flag indicating whether view is currently being scrolled
  bool                            mSortAlphabetically:1;     ///< Sort examples alphabetically.

};

#endif // DALI_DEMO_H
