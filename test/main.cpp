#include "sdl2-wrapper/AVPacketPlayer.h"

void LogBuffer();

int main()
{
	try
	{
		std::filesystem::current_path(Predefine_ResourceDir);
		video::test_AVPacketPlayer();

		// test_SptsEncodeMux();
		// test_tsduck();
		return 0;
	}
	catch (std::runtime_error const &e)
	{
		std::cout << e.what() << std::endl;
		throw;
	}
}

// #include<iostream>
// #include<test_boost_asio.h>
// #include<thread>
//
// int main()
//{
//	try
//	{
//		std::thread([&]()
//		{
//			try
//			{
//				receive_udp_message();
//			}
//			catch (std::exception &e)
//			{
//				std::cerr << "Exception: " << e.what() << "\n";
//			}
//		}).detach();
//
//		std::thread([&]()
//		{
//			try
//			{
//				send_udp_message();
//			}
//			catch (std::exception &e)
//			{
//				std::cerr << "Exception: " << e.what() << "\n";
//			}
//		}).detach();
//
//		std::cin.get();
//	}
//	catch (std::exception &e)
//	{
//		std::cerr << "Exception: " << e.what() << "\n";
//	}
//
//	return 0;
// }
