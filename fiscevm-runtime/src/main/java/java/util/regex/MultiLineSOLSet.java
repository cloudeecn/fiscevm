/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * @author Nikolay A. Kuznetsov
 */
package java.util.regex;

/**
 * Multiline version of the ^ sign.
 * @author Nikolay A. Kuznetsov
 */
class MultiLineSOLSet extends AbstractSet {
    
    private AbstractLineTerminator lt;

    public MultiLineSOLSet(AbstractLineTerminator lt) {
        this.lt = lt;
    }

    public int matches(int strIndex, CharSequence testString,
            MatchResultImpl matchResult) {
        if (strIndex != matchResult.getRightBound()
                && ((strIndex == 0 || (matchResult.hasAnchoringBounds() && strIndex == matchResult
                        .getLeftBound())) || lt.isAfterLineTerminator(
                        testString.charAt(strIndex - 1), testString
                                .charAt(strIndex)))) {
            return next.matches(strIndex, testString, matchResult);
        }

        return -1;

    }

    public boolean hasConsumed(MatchResultImpl matchResult) {
        return false;
    }

    protected String getName() {
        return "^"; //$NON-NLS-1$
    }
}
