/**
 *  Copyright 2010 Yuxuan Huang. All rights reserved.
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.cirnoworks.fisce.intf;

import java.io.InputStream;

import org.w3c.dom.Element;

public interface IToolkit {

	/**
	 * 将这个工具包和虚拟机绑定
	 * 
	 * @param context
	 *            要绑定的虚拟机
	 */
	void setContext(FiScEVM context);

	/**
	 * 工具包对虚拟机进行配置的自定义代码
	 */
	void setupContext();

	/**
	 * 根据文件名获得输入流
	 * 
	 * @param name
	 *            文件名
	 * @return 该类的class文件的输入流
	 */
	InputStream getResourceByName(String name);

	/**
	 * 保存当前状态
	 * 
	 * @param data
	 *            用于保存的xml节点
	 * @throws VMCriticalException
	 *             保存出错了
	 */
	void saveData(Element data) throws VMCriticalException;

	/**
	 * 载入状态
	 * 
	 * @param data
	 *            保存了状态的xml节点
	 * @throws VMCriticalException
	 *             载入出错
	 */
	void loadData(Element data) throws VMCriticalException;
}
