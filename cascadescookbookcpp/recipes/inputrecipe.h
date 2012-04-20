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

#ifndef _INPUTRECIPE_H_
#define _INPUTRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Label;
    }
}

/** 
 * InputRecipe
 * 
 * Shows how to set up different type of text input. The simple one line
 * TextField and a TextArea is shown. How one can listen for text changed
 * signals is also illustrated in this recipe.
 */
class InputRecipe: public bb::cascades::CustomControl
{
Q_OBJECT

public:
    InputRecipe(Container *parent = 0);

public slots:

    //We register this slot to be called when the text in the Label has changed.
    void onTextChanging(const QString &newText);

private:
    Label *mInputLabel;
};

#endif // ifndef _INPUTRECIPE_H_
