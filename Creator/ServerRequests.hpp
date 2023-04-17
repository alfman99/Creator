#pragma once
#include "pch.h"
#include "Cryptography.hpp"

class ServerRequests {
public:
	static ResponseRegisterProject* RegisterProject(string API_KEY);
};