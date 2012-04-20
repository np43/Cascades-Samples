/* Copyright (c) 2012 Research In Motion Limited.
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
#include "speedgauge.h"

#include <bb/cascades/Container>
#include <bb/cascades/ImageView>
#include <bb/cascades/DockLayout>
#include <bb/cascades/DockLayoutProperties>
#include <QDateTime>

#define WORD_LENGTH 6.0f
#define ZERO_DIAL_ANGLE -120.0f

using namespace bb::cascades;

SpeedGauge::SpeedGauge() :
        CustomControl()
{
    Container *content = new Container();

    // The Content Container will be set to layout children using a dock layout (to center everything on screen).
    content->setLayout(new DockLayout());

    // Background image in an ImageView.
    ImageView *bgImage = ImageView::create("asset:///images/dial_bg.png");
    bgImage->setPreferredSize(395, 397);

    // Shine Image in an ImageView.
    ImageView *dialShineImage = ImageView::create("asset:///images/dial_shine.png");
    dialShineImage->setPreferredSize(339, 338);
    dialShineImage->setLayoutProperties(DockLayoutProperties::create()
                                            .horizontal(HorizontalAlignment::Center)
                                            .vertical(VerticalAlignment::Center));

    // Creating the needles, both needles are centered in the dial and should rotate around the base.
    mSpeedNeedle = setUpNeedle(38.0f, 175.0f, "asset:///images/needle.png");
    mMaxNeedle = setUpNeedle(50.0f, 189.0f, "asset:///images/red_needle.png");

    // Adding the controls to the Container.
    content->add(bgImage);
    content->add(mMaxNeedle);
    content->add(mSpeedNeedle);
    content->add(dialShineImage);

    // Set up initial state variable for max speed.
    mMaxSpeedAngle = ZERO_DIAL_ANGLE;

    setRoot(content);
}

ImageView *SpeedGauge::setUpNeedle(float width, float height, const QString pImageUrl)
{
    // Create the needle ImageView, set size and align it to the center of the dial.
    ImageView *needleImage = ImageView::create(pImageUrl);
    needleImage->setPreferredSize(width, height);
    needleImage->setLayoutProperties(DockLayoutProperties::create()
                                        .horizontal(HorizontalAlignment::Center)
                                        .vertical(VerticalAlignment::Center));

    // The center point variable is the point where we want the rotation to be centered
    // normally this is in the exact middle width/2 and height/2 but we want it to be
    // in the bottom of the image, but a little bit up. If you look at the red dot,
    // we want it to be in the center of that.
    float centerPoint = height / 2- width / 2;

    needleImage->setPivotY(centerPoint);
    needleImage->setTranslationY(-centerPoint);

    needleImage->setRotationZ(ZERO_DIAL_ANGLE);

    return needleImage;
}

long SpeedGauge::getTimeMs()
{
    // Returns current system time, used to calculate the speed.
    QDateTime time = QDateTime::currentDateTime();
    return time.toMSecsSinceEpoch () ;
}

void SpeedGauge::calculateSpeed(int nbrOfChars)
{
    long currentTime = getTimeMs();

    if (mStartTime == 0) {
        // If the start time has not been set initialize it.
        mStartTime = getTimeMs();
    }

    // Calculate the number of words by dividing number of characters by an average word length.
    float nbrOfWords = (float) (nbrOfChars / WORD_LENGTH);
    mNbrOfChars = nbrOfChars;

    // We do not start displaying the speed until one full word has been entered. 
    if (nbrOfWords >= 1.0) {

        // Time since start in minutes.
        float time = (currentTime - mStartTime) / (1000.0f * 60.0f);

        // Speed in words / minute.
        float speed = nbrOfWords / time;

        // The speed is translated into an angle (0 speed at -120 degrees and 60 at 120).
        float speedAngle = speed * (-ZERO_DIAL_ANGLE) * 2 / 60.0f + ZERO_DIAL_ANGLE;

        // Rotate the speed needle (using implicit animation).
        mSpeedNeedle->setRotationZ(speedAngle);

        if (speedAngle >= mMaxSpeedAngle) {
            // If the speed is the highest so far update the rotation angle of the max needle.
            mMaxSpeedAngle = speedAngle;
            mMaxNeedle->setRotationZ(speedAngle);
        }
    }
}

float SpeedGauge::stopMeasuring()
{
    // Return the final average speed.
    float average = (60.0f * (mSpeedNeedle->rotationZ() - ZERO_DIAL_ANGLE) / (-ZERO_DIAL_ANGLE * 2.0f));
    return average;
}

