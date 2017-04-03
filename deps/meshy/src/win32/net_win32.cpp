/**
 * licensed to the apache software foundation (asf) under one
 * or more contributor license agreements.  see the notice file
 * distributed with this work for additional information
 * regarding copyright ownership.  the asf licenses this file
 * to you under the apache license, version 2.0 (the
 * "license"); you may not use this file except in compliance
 * with the license.  you may obtain a copy of the license at
 *
 * http://www.apache.org/licenses/license-2.0
 *
 * unless required by applicable law or agreed to in writing, software
 * distributed under the license is distributed on an "as is" basis,
 * without warranties or conditions of any kind, either express or implied.
 * see the license for the specific language governing permissions and
 * limitations under the license.
 */

#include "win32/net_win32.h"
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

namespace meshy {

	void WindowsSocketInitializer::Initialize()
	{
		static WindowsSocketInitializer initalizer;
	}

	WindowsSocketInitializer::WindowsSocketInitializer() {
		// WinSock 2.2
		WORD wVersionRequested = MAKEWORD(2, 2);

		WSADATA wsaData;
		DWORD err = WSAStartup(wVersionRequested, &wsaData);
		if ( 0 != err ) {
			std::cerr << "Request Windows Socket Library Error!" << std::endl;
			throw std::exception("Request Windows Socket Library Error!");
		}

		// Check the library version
		if ( LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 ) {
			WSACleanup();
			std::cerr << "Request Windows Socket Version 2.2 Error!" << std::endl;
			throw std::exception("Request Windows Socket Version 2.2 Error!");
		}
	}

}