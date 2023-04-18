#include "pch.h"
#include "ServerRequests.hpp"

ResponseRegisterProject* ServerRequests::RegisterProject(string API_KEY) {
	httplib::Client cli("http://localhost:3000");
	auto res = cli.Get("/api/registerProject?API_KEY=" + API_KEY);
	if (res.error() != httplib::Error::Success) return nullptr;
	if (res->status != 200) return nullptr;

	ResponseRegisterProject* response = new ResponseRegisterProject();

	memcpy(response, res->body.data(), sizeof(ResponseRegisterProject));

	return response;
}
