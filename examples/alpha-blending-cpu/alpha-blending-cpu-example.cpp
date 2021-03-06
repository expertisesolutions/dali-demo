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

#include <dali-toolkit/dali-toolkit.h>
#include <cstring>

using namespace Dali;

namespace
{
const char* const IMAGE_PATH_1(DEMO_IMAGE_DIR "people-small-7b.jpg"); // 100x100
const char* const IMAGE_PATH_2(DEMO_IMAGE_DIR "people-medium-7.jpg");
const char* const IMAGE_PATH_3(DEMO_IMAGE_DIR "people-medium-7-rgb565.png");    // is compressed
const char* const IMAGE_PATH_4(DEMO_IMAGE_DIR "people-medium-7-masked.png");    // has alpha channel
const char* const MASK_IMAGE_PATH_1(DEMO_IMAGE_DIR "store_mask_profile_n.png"); // 300x300
const char* const MASK_IMAGE_PATH_2(DEMO_IMAGE_DIR "store_mask_profile_f.png");
} // namespace

class ImageViewAlphaBlendApp : public ConnectionTracker
{
public:
  ImageViewAlphaBlendApp(Application& application)
  : mApplication(application),
    mImageCombinationIndex(0)
  {
    // Connect to the Application's Init signal
    mApplication.InitSignal().Connect(this, &ImageViewAlphaBlendApp::Create);
  }

  ~ImageViewAlphaBlendApp()
  {
    // Nothing to do here;
  }

private:
  // The Init signal is received once (only) during the Application lifetime
  void Create(Application& application)
  {
    // This creates an image view with one of 3 images, and one of 2 masks.
    // Clicking the screen will cycle through each combination of mask and image.

    // Get a handle to the window
    Window window = application.GetWindow();
    window.KeyEventSignal().Connect(this, &ImageViewAlphaBlendApp::OnKeyEvent);
    window.SetBackgroundColor(Color::WHITE);

    mImageView = Toolkit::ImageView::New();

    mImageView.SetProperty(Actor::Property::SIZE, Vector2(200, 200));
    mImageView.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::CENTER);
    window.Add(mImageView);

    mImageLabel = Toolkit::TextLabel::New();
    mImageLabel.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::BOTTOM_CENTER);
    mImageLabel.SetProperty(Actor::Property::ANCHOR_POINT, ParentOrigin::BOTTOM_CENTER);
    mImageLabel.SetProperty(Actor::Property::POSITION, Vector3(0.0f, -50.0f, 0.0f));
    mImageLabel.SetProperty(Toolkit::TextLabel::Property::TEXT_COLOR, Color::BLACK);
    window.Add(mImageLabel);

    mMaskLabel = Toolkit::TextLabel::New();
    mMaskLabel.SetProperty(Actor::Property::PARENT_ORIGIN, ParentOrigin::BOTTOM_CENTER);
    mMaskLabel.SetProperty(Actor::Property::ANCHOR_POINT, ParentOrigin::BOTTOM_CENTER);
    mMaskLabel.SetProperty(Actor::Property::POSITION, Vector3(0.0f, 0.0f, 0.0f));
    mMaskLabel.SetProperty(Toolkit::TextLabel::Property::TEXT_COLOR, Color::BLACK);
    window.Add(mMaskLabel);

    LoadImages();

    window.TouchedSignal().Connect(this, &ImageViewAlphaBlendApp::OnTouched);
  }

  void OnTouched(const TouchEvent& touch)
  {
    static bool touched = false;
    if(touch.GetState(0) == PointState::DOWN)
    {
      touched = true;
    }

    if(touch.GetState(0) == PointState::UP && touched)
    {
      mImageCombinationIndex++;
      touched = false;
      LoadImages();
    }
  }

  void LoadImages()
  {
    const char* images[4] = {IMAGE_PATH_1, IMAGE_PATH_2, IMAGE_PATH_3, IMAGE_PATH_4};
    const char* masks[2]  = {MASK_IMAGE_PATH_1, MASK_IMAGE_PATH_2};

    const char* mask  = masks[mImageCombinationIndex % 2];        // Cycle through masks
    const char* image = images[(mImageCombinationIndex / 2) % 4]; // then images

    Property::Map map;
    map.Add(Toolkit::Visual::Property::TYPE, Toolkit::Visual::Type::IMAGE);
    map.Add(Toolkit::ImageVisual::Property::URL, image);
    map.Add(Toolkit::ImageVisual::Property::ALPHA_MASK_URL, mask);

    if(mImageCombinationIndex % 2 == 0)
    {
      map.Add(Toolkit::ImageVisual::Property::MASK_CONTENT_SCALE, 1.f);
      map.Add(Toolkit::ImageVisual::Property::CROP_TO_MASK, false);
    }
    else
    {
      map.Add(Toolkit::ImageVisual::Property::MASK_CONTENT_SCALE, 1.6f);
      map.Add(Toolkit::ImageVisual::Property::CROP_TO_MASK, true);
    }

    mImageView.SetProperty(Toolkit::ImageView::Property::IMAGE, map);

    mImageLabel.SetProperty(Toolkit::TextLabel::Property::TEXT, strrchr(image, '/'));
    mMaskLabel.SetProperty(Toolkit::TextLabel::Property::TEXT, strrchr(mask, '/'));
  }

  void OnKeyEvent(const KeyEvent& event)
  {
    if(event.GetState() == KeyEvent::DOWN)
    {
      if(IsKey(event, Dali::DALI_KEY_ESCAPE) || IsKey(event, Dali::DALI_KEY_BACK))
      {
        mApplication.Quit();
      }
    }
  }

private:
  Application&       mApplication;
  Toolkit::ImageView mImageView;
  Toolkit::TextLabel mImageLabel;
  Toolkit::TextLabel mMaskLabel;

  int mImageCombinationIndex;
};

int DALI_EXPORT_API main(int argc, char** argv)
{
  Application            application = Application::New(&argc, &argv);
  ImageViewAlphaBlendApp test(application);
  application.MainLoop();
  return 0;
}
