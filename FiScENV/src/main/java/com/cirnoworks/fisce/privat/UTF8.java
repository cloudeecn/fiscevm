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
package com.cirnoworks.fisce.privat;

public final class UTF8 {
	public static int utf8Size(char unicode) {
		if (unicode > 0x0800) {
			return 3;
		} else if (unicode > 0x80 || unicode == 0) {
			return 2;
		} else {
			return 1;
		}
	}

	public static int utf8SizeU(byte firstByte) {
		if (firstByte >= 0) {
			return 1;
		} else if (firstByte >= -16) {
			return 0;
		} else if (firstByte >= -32) {
			return 3;
		} else if (firstByte >= -64) {
			return 2;
		} else {
			return 0;
		}
	}
}
