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

// INTERNAL INCLUDES
#include "shared/view.h"

#include <dali-toolkit/dali-toolkit.h>
#include <dali-toolkit/devel-api/controls/shadow-view/shadow-view.h>
#include <dali/dali.h>

#include <iostream>

using namespace Dali;
using namespace Dali::Toolkit;
using std::string;
using namespace DemoHelper;

namespace
{
const char* BACKGROUND_IMAGE(DEMO_IMAGE_DIR "background-default.png");
const char* TOOLBAR_IMAGE(DEMO_IMAGE_DIR "top-bar.png");

const char* APPLICATION_TITLE_PAN_LIGHT("Lighting: Pan Light");
const char* APPLICATION_TITLE_ROTATE_OBJECT("Lighting: Rotate Object");
const char* APPLICATION_TITLE_PAN_SCENE("Lighting: Pan Scene");
const char* APPLICATION_TITLE_ROTATE_SCENE("Lighting: Rotate Scene");
const char* CHANGE_EFFECT_IMAGE(DEMO_IMAGE_DIR "icon-change.png");
const char* CHANGE_EFFECT_IMAGE_SELECTED(DEMO_IMAGE_DIR "icon-change-selected.png");
const char* RESET_ICON(DEMO_IMAGE_DIR "icon-reset.png");
const char* RESET_ICON_SELECTED(DEMO_IMAGE_DIR "icon-reset-selected.png");

const char* SCENE_IMAGE_1(DEMO_IMAGE_DIR "gallery-small-10.jpg");
const char* SCENE_IMAGE_2(DEMO_IMAGE_DIR "gallery-small-42.jpg");
const char* SCENE_IMAGE_3(DEMO_IMAGE_DIR "gallery-small-48.jpg");

const float MIN_PINCH_SCALE(0.3f);
const float MAX_PINCH_SCALE(2.05f);

const float   R3_2(0.8660254);
const Vector3 TOP_POINT(0.0f, -1.0f, 0.0f);
const Vector3 LEFT_POINT(-R3_2, 0.5f, 0.0f);
const Vector3 RIGHT_POINT(R3_2, 0.5f, 0.0f);
const Vector3 FRONT_POINT(0.0f, 0.0f, 20.0f);

const Vector2 DEFAULT_WINDOW_SIZE(480.0f, 800.0f);

const float X_ROTATION_DISPLACEMENT_FACTOR  = 60.f;
const float Y_ROTATION_DISPLACEMENT_FACTOR  = 60.f;
const float LIGHT_PAN_X_DISPLACEMENT_FACTOR = 1 / 360.f;
const float LIGHT_PAN_Y_DISPLACEMENT_FACTOR = 1 / 360.f;

} // namespace

/**
 * This example shows a fixed point light onto an animating set of images
 * casting a shadow onto a wall. The whole scene can be rotated.
 */

class TestApp : public ConnectionTracker
{
public:
  /**
   * Constructor
   * @param application class, stored as reference
   */
  TestApp(Application& app)
  : mApp(app),
    mView(),
    mContents(),
    mSceneActor(),
    mAnimation(),
    mSceneAnimation(),
    mPaused(false),
    mShadowView(),
    mShadowPlaneBg(),
    mShadowPlane(),
    mCastingLight(),
    mLightAnchor(),
    mImageActor1(),
    mImageActor2(),
    mImageActor3(),
    mPanGestureDetector(),
    mPinchGestureDetector(),
    mTapGestureDetector(),
    mTranslation(22.0f, -1.0f, 0.0f),
    mSceneXRotation(Degree(-6.0f)), // Initial values give a reasonable off-straight view.
    mSceneYRotation(Degree(20.0f)),
    mLightXRotation(Degree(-1.5f)),
    mLightYRotation(Degree(-9.5f)),
    mObjectXRotation(0.0f),
    mObjectYRotation(0.0f),
    mPinchScale(0.6f),
    mScaleAtPinchStart(0.6f),
    mAngle1Index(Property::INVALID_INDEX),
    mAngle3Index(Property::INVALID_INDEX),
    mTitleActor(),
    mPanState(PAN_LIGHT)
  {
    app.InitSignal().Connect(this, &TestApp::Create);
    app.TerminateSignal().Connect(this, &TestApp::Terminate);
  }

  ~TestApp()
  {
    // Nothing to do here; All the members of this class get deleted automatically and they delete their children
  }

public:
  struct RotationConstraint
  {
    RotationConstraint(float sign)
    : mSign(sign)
    {
    }

    void operator()(Quaternion& current, const PropertyInputContainer& inputs)
    {
      Radian angle(inputs[0]->GetFloat());
      current = Quaternion(angle * mSign, Vector3::YAXIS);
    }

    float mSign;
  };

  /**
   * This method gets called once the main loop of application is up and running
   */
  void Create(Application& app)
  {
    srand(0); // Want repeatable path

    app.GetWindow().KeyEventSignal().Connect(this, &TestApp::OnKeyEvent);

    CreateToolbarAndView(app);
    CreateShadowViewAndLights();
    CreateScene();
  }

  void CreateToolbarAndView(Application& app)
  {
    // Creates a default view with a default tool bar.
    // The view is added to the window.
    Toolkit::ToolBar toolBar;
    mContents = DemoHelper::CreateView(app,
                                       mView,
                                       toolBar,
                                       BACKGROUND_IMAGE,
                                       TOOLBAR_IMAGE,
                                       "");

    // Add an effect-changing button on the right of the tool bar.
    Toolkit::PushButton effectChangeButton = Toolkit::PushButton::New();
    effectChangeButton.SetProperty(Toolkit::Button::Property::UNSELECTED_BACKGROUND_VISUAL, CHANGE_EFFECT_IMAGE);
    effectChangeButton.SetProperty(Toolkit::Button::Property::SELECTED_BACKGROUND_VISUAL, CHANGE_EFFECT_IMAGE_SELECTED);
    effectChangeButton.ClickedSignal().Connect(this, &TestApp::OnEffectButtonClicked);
    toolBar.AddControl(effectChangeButton, DemoHelper::DEFAULT_VIEW_STYLE.mToolBarButtonPercentage, Toolkit::Alignment::HORIZONTAL_RIGHT, DemoHelper::DEFAULT_MODE_SWITCH_PADDING);

    // Add title to the tool bar.
    mTitleActor = DemoHelper::CreateToolBarLabel("");
    toolBar.AddControl(mTitleActor, DemoHelper::DEFAULT_VIEW_STYLE.mToolBarTitlePercentage, Toolkit::Alignment::HORIZONTAL_CENTER);

    // Set Title text
    mTitleActor.SetProperty(TextLabel::Property::TEXT, std::string(APPLICATION_TITLE_PAN_LIGHT));

    //Add a reset button
    Toolkit::PushButton resetButton = Toolkit::PushButton::New();
    resetButton.SetProperty(Toolkit::Button::Property::UNSELECTED_BACKGROUND_VISUAL, RESET_ICON);
    resetButton.SetProperty(Toolkit::Button::Property::SELECTED_BACKGROUND_VISUAL, RESET_ICON_SELECTED);
    resetButton.ClickedSignal().Connect(this, &TestApp::OnResetPressed);
    toolBar.AddControl(resetButton, DemoHelper::DEFAULT_VIEW_STYLE.mToolBarButtonPercentage, Toolkit::Alignment::HORIZONTAL_CENTER, DemoHelper::DEFAULT_PLAY_PADDING);

    // Setup
    mView.SetProperty(Actor::Property::POSITION, Vector3(0.0f, 0.0f, 0.0f));

    mContents.SetProperty(Layer::Property::BEHAVIOR, Layer::LAYER_3D);
    mContents.SetProperty(Actor::Property::POSITION, mTranslation);
    mContents.SetProperty(Actor::Property::ORIENTATION, CalculateWorldRotation(mSceneXRotation, mSceneYRotation));
    mContents.SetProperty(Actor::Property::SCALE, Vector3(mPinchScale, mPinchScale, mPinchScale));

    mPanGestureDetector = PanGestureDetector::New();
    mPanGestureDetector.Attach(mView);
    mPanGestureDetector.DetectedSignal().Connect(this, &TestApp::OnPan);

    mPinchGestureDetector = PinchGestureDetector::New();
    mPinchGestureDetector.Attach(mView);
    mPinchGestureDetector.DetectedSignal().Connect(this, &TestApp::OnPinch);

    mTapGestureDetector = TapGestureDetector::New();
    mTapGestureDetector.Attach(mView);
    mTapGestureDetector.DetectedSignal().Connect(this, &TestApp::OnTap);
  }

  void CreateShadowViewAndLights()
  {
    mShadowView = Toolkit::ShadowView::New();
    mShadowView.SetProperty(Dali::Actor::Property::NAME, "Container");
    mShadowView.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
    mShadowView.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
    mShadowView.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::ALL_DIMENSIONS);
    mShadowView.SetPointLightFieldOfView(Math::PI / 2.0f);
    mContents.Add(mShadowView);

    mShadowPlaneBg = ImageView::New(DEMO_IMAGE_DIR "brick-wall.jpg");
    mShadowPlaneBg.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
    mShadowPlaneBg.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
    mShadowPlaneBg.SetProperty(Dali::Actor::Property::NAME, "Plane");
    mShadowPlaneBg.SetProperty(Actor::Property::SIZE, Vector2(1000.0f, 1000.0f));
    mContents.Add(mShadowPlaneBg);
    mShadowPlaneBg.SetProperty(Actor::Property::POSITION, Vector3(50.0f, 50.0f, -200.0f));

    mShadowView.SetShadowPlaneBackground(mShadowPlaneBg);
    mShadowView.Activate();

    mLightAnchor = Actor::New();
    mLightAnchor.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
    mLightAnchor.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
    mLightAnchor.SetProperty(Actor::Property::ORIENTATION, CalculateWorldRotation(mLightXRotation, mLightYRotation));

    // Work out a scaling factor as the initial light position was calculated for desktop
    // Need to scale light position as scene actor size is based on window size (i.e. much bigger on device)
    Vector2 windowSize(mApp.GetWindow().GetSize());
    float   scaleFactor = windowSize.x / DEFAULT_WINDOW_SIZE.x;

    mCastingLight = Actor::New();
    mCastingLight.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
    mCastingLight.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER);
    mCastingLight.SetProperty(Actor::Property::POSITION, Vector3(0.0f, 0.0f, 800.0f) * scaleFactor);

    TextLabel text = TextLabel::New("Light");
    text.SetProperty(TextLabel::Property::POINT_SIZE, 20.0f);
    text.SetResizePolicy(ResizePolicy::USE_NATURAL_SIZE, Dimension::ALL_DIMENSIONS);
    text.SetProperty(Actor::Property::COLOR, Color::BLUE);

    mCastingLight.Add(text);
    mLightAnchor.Add(mCastingLight);
    mShadowPlaneBg.Add(mLightAnchor);

    text.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
    mShadowView.SetPointLight(mCastingLight);
  }

  void CreateScene()
  {
    mSceneActor = Actor::New();
    mSceneActor.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);

    // Create and add images to the scene actor:
    mImageActor1 = ImageView::New(SCENE_IMAGE_1);
    mImageActor2 = ImageView::New(SCENE_IMAGE_2);
    mImageActor3 = ImageView::New(SCENE_IMAGE_3);

    mImageActor1.SetResizePolicy(ResizePolicy::USE_NATURAL_SIZE, Dimension::ALL_DIMENSIONS);
    mImageActor2.SetResizePolicy(ResizePolicy::USE_NATURAL_SIZE, Dimension::ALL_DIMENSIONS);
    mImageActor3.SetResizePolicy(ResizePolicy::USE_NATURAL_SIZE, Dimension::ALL_DIMENSIONS);

    mImageActor2.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);

    mImageActor1.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER_LEFT);
    mImageActor1.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER_RIGHT);

    mImageActor3.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER_RIGHT);
    mImageActor3.SetProperty(Actor::Property::ANCHOR_POINT, AnchorPoint::CENTER_LEFT);

    mSceneActor.Add(mImageActor2);
    mImageActor2.Add(mImageActor1);
    mImageActor2.Add(mImageActor3);

    Property::Index angleIndex = mImageActor2.RegisterProperty("angle", Property::Value(Dali::ANGLE_30));
    Source          angleSrc(mImageActor2, angleIndex);

    Constraint constraint = Constraint::New<Quaternion>(mImageActor1, Actor::Property::ORIENTATION, RotationConstraint(-1.0f));
    constraint.AddSource(angleSrc);
    constraint.Apply();

    constraint = Constraint::New<Quaternion>(mImageActor3, Actor::Property::ORIENTATION, RotationConstraint(+1.0f));
    constraint.AddSource(angleSrc);
    constraint.Apply();

    mSceneAnimation = Animation::New(2.5f);

    // Want to animate angle from 30 => -30 and back again smoothly.

    mSceneAnimation.AnimateTo(Property(mImageActor2, angleIndex), Property::Value(-Dali::ANGLE_30), AlphaFunction::SIN);

    mSceneAnimation.SetLooping(true);
    mSceneAnimation.Play();

    mSceneActor.SetProperty(Actor::Property::SIZE, Vector2(250.0f, 250.0f));
    mSceneActor.SetProperty(Actor::Property::POSITION, Vector3(0.0f, 0.0f, 130.0f));
    mShadowView.Add(mSceneActor);
  }

  Quaternion CalculateWorldRotation(Radian XRotation, Radian YRotation)
  {
    Quaternion p(XRotation, Vector3::XAXIS);
    Quaternion q(YRotation, Vector3::YAXIS);
    return p * q;
  }

  void OnTap(Dali::Actor actor, const TapGesture& gesture)
  {
    if(mSceneAnimation)
    {
      if(!mPaused)
      {
        mSceneAnimation.Pause();
        mPaused = true;
      }
      else
      {
        mSceneAnimation.Play();
        mPaused = false;
      }
    }
  }

  void OnPan(Actor actor, const PanGesture& gesture)
  {
    switch(gesture.GetState())
    {
      case GestureState::CONTINUING:
      {
        const Vector2& displacement = gesture.GetDisplacement();
        switch(mPanState)
        {
          case PAN_LIGHT:
          {
            mLightXRotation = mLightXRotation - displacement.y * LIGHT_PAN_X_DISPLACEMENT_FACTOR; // X displacement rotates around Y axis
            mLightXRotation = Clamp(mLightXRotation, -Dali::ANGLE_45, Dali::ANGLE_45);
            mLightYRotation = mLightYRotation + displacement.x * LIGHT_PAN_Y_DISPLACEMENT_FACTOR; // Y displacement rotates around X axis
            mLightYRotation = Clamp(mLightYRotation, -Dali::ANGLE_45, Dali::ANGLE_45);
            mLightAnchor.SetProperty(Actor::Property::ORIENTATION, CalculateWorldRotation(mLightXRotation, mLightYRotation));
            break;
          }

          case PAN_SCENE:
          {
            mTranslation += Vector3(displacement.x, displacement.y, 0.f);
            mContents.SetProperty(Actor::Property::POSITION, mTranslation);
            break;
          }

          case ROTATE_SCENE:
          {
            mSceneXRotation = mSceneXRotation - displacement.y / X_ROTATION_DISPLACEMENT_FACTOR; // X displacement rotates around Y axis
            mSceneXRotation = Clamp(mSceneXRotation, -Dali::ANGLE_90, Dali::ANGLE_90);
            mSceneYRotation = mSceneYRotation + displacement.x / Y_ROTATION_DISPLACEMENT_FACTOR; // Y displacement rotates around X axis
            mSceneYRotation = Clamp(mSceneYRotation, -Dali::ANGLE_90, Dali::ANGLE_90);
            mContents.SetProperty(Actor::Property::ORIENTATION, CalculateWorldRotation(mSceneXRotation, mSceneYRotation));
            break;
          }

          case ROTATE_OBJECT:
          {
            mObjectXRotation = mObjectXRotation - displacement.y / X_ROTATION_DISPLACEMENT_FACTOR; // X displacement rotates around Y axis
            mObjectYRotation = mObjectYRotation + displacement.x / Y_ROTATION_DISPLACEMENT_FACTOR; // Y displacement rotates around X axis
            mSceneActor.SetProperty(Actor::Property::ORIENTATION, CalculateWorldRotation(mObjectXRotation, mObjectYRotation));
            break;
          }
        }
      }
      break;

      case GestureState::FINISHED:
        // Start animation at last known speed
        break;

      default:
        break;
    }
  }

  void OnPinch(Actor actor, const PinchGesture& gesture)
  {
    if(gesture.GetState() == GestureState::STARTED)
    {
      mScaleAtPinchStart = mContents.GetCurrentProperty<Vector3>(Actor::Property::SCALE).x;
    }
    mPinchScale = Clamp(mScaleAtPinchStart * gesture.GetScale(), MIN_PINCH_SCALE, MAX_PINCH_SCALE);

    mContents.SetProperty(Actor::Property::SCALE, Vector3(mPinchScale, mPinchScale, mPinchScale));
  }

  void Terminate(Application& app)
  {
    if(mSceneActor)
    {
      mApp.GetWindow().Remove(mSceneActor);
    }
    if(mView)
    {
      mApp.GetWindow().Remove(mView);
    }
  }

  void OnKeyEvent(const KeyEvent& event)
  {
    if(event.GetState() == KeyEvent::DOWN)
    {
      if(IsKey(event, Dali::DALI_KEY_ESCAPE) || IsKey(event, Dali::DALI_KEY_BACK))
      {
        mApp.Quit();
      }
    }
  }

  bool OnEffectButtonClicked(Toolkit::Button button)
  {
    switch(mPanState)
    {
      case PAN_LIGHT:
        mPanState = ROTATE_OBJECT;
        mTitleActor.SetProperty(TextLabel::Property::TEXT, std::string(APPLICATION_TITLE_ROTATE_OBJECT));
        break;
      case ROTATE_OBJECT:
        mPanState = ROTATE_SCENE;
        mTitleActor.SetProperty(TextLabel::Property::TEXT, std::string(APPLICATION_TITLE_ROTATE_SCENE));
        break;
      case ROTATE_SCENE:
        mPanState = PAN_SCENE;
        mTitleActor.SetProperty(TextLabel::Property::TEXT, std::string(APPLICATION_TITLE_PAN_SCENE));
        break;
      case PAN_SCENE:
        mPanState = PAN_LIGHT;
        mTitleActor.SetProperty(TextLabel::Property::TEXT, std::string(APPLICATION_TITLE_PAN_LIGHT));
        break;
      default:
        break;
    }

    return true;
  }

  bool OnResetPressed(Toolkit::Button button)
  {
    // Reset translation
    mTranslation = Vector3::ZERO;
    mContents.SetProperty(Actor::Property::POSITION, mTranslation);

    // Align scene so that light anchor orientation is Z Axis
    mSceneXRotation = -mLightXRotation;
    mSceneYRotation = -mLightYRotation;
    mContents.SetProperty(Actor::Property::ORIENTATION, CalculateWorldRotation(mSceneXRotation, mSceneYRotation));

    return true;
  }

private:
  Application&         mApp;
  Toolkit::Control     mView;
  Layer                mContents;
  Actor                mSceneActor;
  Animation            mAnimation;
  Animation            mSceneAnimation;
  bool                 mPaused;
  Toolkit::ShadowView  mShadowView;
  ImageView            mShadowPlaneBg;
  ImageView            mShadowPlane;
  Actor                mCastingLight;
  Actor                mLightAnchor;
  ImageView            mImageActor1;
  ImageView            mImageActor2;
  ImageView            mImageActor3;
  PanGestureDetector   mPanGestureDetector;
  PinchGestureDetector mPinchGestureDetector;
  TapGestureDetector   mTapGestureDetector;
  Vector3              mTranslation;
  Radian               mSceneXRotation;
  Radian               mSceneYRotation;
  Radian               mLightXRotation;
  Radian               mLightYRotation;
  Radian               mObjectXRotation;
  Radian               mObjectYRotation;
  float                mPinchScale;
  float                mScaleAtPinchStart;

  Property::Index mAngle1Index;
  Property::Index mAngle3Index;

  Toolkit::TextLabel mTitleActor;

  enum PanState
  {
    PAN_SCENE,
    ROTATE_SCENE,
    PAN_LIGHT,
    ROTATE_OBJECT
  };

  PanState mPanState;
};

/*****************************************************************************/

int DALI_EXPORT_API main(int argc, char** argv)
{
  Application app = Application::New(&argc, &argv, DEMO_THEME_PATH);
  TestApp     theApp(app);
  app.MainLoop();
  return 0;
}
