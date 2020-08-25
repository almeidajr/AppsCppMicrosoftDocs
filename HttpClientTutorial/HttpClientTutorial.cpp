// HttpClientTutorial.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

int main() {
	auto fileStream = std::make_shared<ostream>();

	auto requestTask = fstream::open_ostream(U("results.html"))
						   .then([=](ostream outFile) {
							   *fileStream = outFile;

							   http_client client(U("http://www.bing.com/"));

							   uri_builder builder(U("/search"));
							   builder.append_query(U("q"), U("cpprestsdk github"));

							   return client.request(methods::GET, builder.to_string());
						   })
						   .then([=](http_response response) {
							   std::cout << R"(Received response status code: )"
										 << response.status_code() << std::endl;

							   return response.body().read_to_end(fileStream->streambuf());
						   })
						   .then([=](size_t) {
							   return fileStream->close();
						   });

	try {
		requestTask.wait();
	}
	catch (const std::exception& e) {
		std::cout << R"(Error exception: )" << e.what() << std::endl;
	}

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
