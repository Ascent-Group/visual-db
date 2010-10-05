/*-
 * Copyright (c) 2009, Ascent Group.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *     * Neither the name of the Ascent Group nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GRAPHICSSCENETEST_H
#define GRAPHICSSCENETEST_H

#include <QtTest/QtTest>

class GraphicsSceneTest : public QObject
{
    Q_OBJECT

    private slots:
        void initTestCase();
        void cleanupTestCase();

        void addTableItemTest();
        void adjustTablesTest();
        void cleanTableSchemeSceneTest();
        void colorizeAccordingSchemasTest();
        void contextMenuEventTest();
        void createItemGroupTest();
        void createRelationsTest();
        void deleteTableItemTest();
        void divideOnPagesTest();
        void drawBackgroundTest();
        void drawGridTest();
        void findTableItemTest();
        void groupItemsTest();
        void mouseMoveEventTest();
        void mousePressEventTest();
        void mouseReleaseEventTest();
        void moveDownTest();
        void moveLeftTest();
        void moveLegendTest();
        void moveModeTest();
        void moveRightTest();
        void moveUpTest();
        void movedDownTest();
        void movedLeftTest();
        void movedRightTest();
        void movedUpTest();
        void refreshLegendTest();
        void resizeTest();
        void selectAllTablesInSchemaTest();
        void selectAllTablesTest();
        void setAcceptsHoverEventsTest();
        void setFieldsTypesInvisibleTest();
        void setFieldsTypesVisibleTest();
        void setIndicesVisibleTest();
        void setMoveModeTest();
        void setSchemeMenuTest();
        void setTableColorTest();
        void setTableMenuTest();
        void settedMoveModeTest();
        void showGridTest();
        void showLegendTest();
        void showOnSceneTest();
        void ungroupItemsTest();
        void updateLegendTest();
};
#endif // GRAPHICSSCENETEST_H
