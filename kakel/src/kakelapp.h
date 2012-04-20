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
#ifndef KAKELAPP_H
#define KAKELAPP_H

#include <bb/cascades/Application>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Container;
        class ImageView;
        class Page;
        class TouchEvent;
    }
}

/**
 * This is a puzzle game testing you logical skills. The game has some
 * decorative and interaction elements set up in QML, while the actual
 * game board and logics is handled in C++.
 *
 * You will learn how to:
 * - Load part of the UI in QML and part of it in C++.
 * - Dynamically change the part loaded in C++.
 */
class KakelApp: public QObject
{
Q_OBJECT

public:
    KakelApp();
    ~KakelApp();

    /**
     * Invokable function used for resetting the tile layout from QML.
     */
    Q_INVOKABLE void resetTiles();

    /**
     * Invokable function used for setting the number of tiles from QM.
     */
    Q_INVOKABLE void setNumTiles(int numTiles);

    /**
     * The numMoves is a property that we want to be able to listen to in the QML.
     */
    Q_PROPERTY(int numMoves READ numMoves WRITE setNumMoves NOTIFY numMovesChanged)

    /**
     * Getters and setters for the numMoves property.
     */
    void setNumMoves(const int i);
    int numMoves();

private slots:

    /**
     * This slot is connected to the the tiles so we can find out what tiles to turn.
     */
    void onTileTouch(bb::cascades::TouchEvent *event);

signals:

    /**
     * Signal connected to the numMoves property.
     */
    void numMovesChanged();

private:

    /**
     * Creates the PlayArea with all the tiles, it's the setup function that would be
     * tedious to make in QML, but is easy to make in C++.
     */
    void findPlayAreaAndInitialize(Page *appPage);

    /**
     * These functions initialize handle and finds the tiles that the user presses on.
     * The work like this:
     * First of when we get a touch on a tile, we check that tile to see what type it is,
     * if it's already turned into the neutral tile we do nothing but if it's not
     * we remember what tile it was and check the whole playArea for turned tiles.
     * Then if we find turned tiles we check that tiles neighbors for the tiles that
     * look just like the tile that was pressed on. If we find that, we check that ones
     * neighbors. During this process we find the one we originally got a touch on
     * as a turned one.
     */
    void checkPlayAreaAndChange(const QString &msg);
    void checkNeighboursAndChange(int i, int ii, const QString &tileName);
    void checkAndChangeTile(int i, int ii, const QString &tileName);

    // Some Controls that we need to remember.
    Container *mPlayAreaContainer;
    ImageView* mPlayField[32][32]; //32 is the maxSize
    Page *mAppPage;

    // Internal game state variables.
    int mNumMoves;
    float mWantedSize;
    float mNumTiles;
};

#endif // ifndef KAKELAPP_H
