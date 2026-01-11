#include "pch.h"
#include "CppUnitTest.h"
#include "udpCommand.h"

#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(VisualFlightPathExtensionTests)
{
public:
	
	TEST_METHOD(TestParseTooShortMessage)
	{
		char rawBytes[] = {0};

		try
		{
			CommandConfigurationParser::parse(rawBytes, 1);
			Assert::Fail(L"Expected std::invalid_argument was not thrown");
		}
		catch (const std::invalid_argument e)
		{
			Assert::IsTrue(strcmp(e.what(), "missing_command") == 0);
		}
	}

	TEST_METHOD(TestParseInvalidCommandID)
	{
		char rawBytes[] = { 1, 0 };

		try
		{
			CommandConfigurationParser::parse(rawBytes, 2);
			Assert::Fail(L"Expected std::invalid_argument was not thrown");
		}
		catch (const std::invalid_argument e)
		{
			Assert::IsTrue(strcmp(e.what(), "unknown_command") == 0);
		}
	}

	TEST_METHOD(TestRemoveCommandWithoutIndicatorID)
	{
		char rawBytes[] = { 0, 2 };

		std::unique_ptr<AbstractCommandConfiguration> abstractCommand = CommandConfigurationParser::parse(rawBytes, 2);

		Assert::IsTrue(REMOVE == abstractCommand->getCommand());
		
		RemoveIndicatorsCommandConfiguration* removeCommand = static_cast<RemoveIndicatorsCommandConfiguration*>(abstractCommand.get());
		Assert::IsTrue(removeCommand->getIDsToRemove().size() == 0);
	}

	TEST_METHOD(TestRemoveCommandWithInvalidMessageLength)
	{
		char rawBytes[] = { 0, 2, // command
							0};   // other content

		try
		{
			CommandConfigurationParser::parse(rawBytes, 3);
			Assert::Fail(L"Expected std::invalid_argument was not thrown");
		}
		catch (const std::invalid_argument e)
		{
			Assert::IsTrue(strcmp(e.what(), "remove_invalid_length") == 0);
		}
	}

	TEST_METHOD(TestRemoveCommandWithParameters)
	{
		char rawBytes[] = { 0, 2,  // command
							0, 1,  // ID (1)
							0, 2,  // ID (2)
							0, 3}; // ID (3)

		std::unique_ptr<AbstractCommandConfiguration> abstractCommand = CommandConfigurationParser::parse(rawBytes, 8);

		Assert::IsTrue(REMOVE == abstractCommand->getCommand());

		RemoveIndicatorsCommandConfiguration* removeCommand = static_cast<RemoveIndicatorsCommandConfiguration*>(abstractCommand.get());
		Assert::IsTrue(removeCommand->getIDsToRemove().size() == 3);
		Assert::IsTrue(removeCommand->getIDsToRemove().at(0) == 1);
		Assert::IsTrue(removeCommand->getIDsToRemove().at(1) == 2);
		Assert::IsTrue(removeCommand->getIDsToRemove().at(2) == 3);
	}

	TEST_METHOD(TestSetCommandWithInvalidLength)
	{
		char rawBytes[] = { 0, 1,  // command
							0, 1};  // other content

		try
		{
			CommandConfigurationParser::parse(rawBytes, 3);
			Assert::Fail(L"Expected std::invalid_argument was not thrown");
		}
		catch (const std::invalid_argument e)
		{
			Assert::IsTrue(strcmp(e.what(), "set_invalid_length") == 0);
		}
	}

	TEST_METHOD(TestSetCommandWithValidValues)
	{
		char rawBytes[] = { 0, 1,					// command
							0, 1,					// Indicator ID
							0, 0, 0, 1,				// Indicator Type ID
							0, 0, 0, 0, 0, 0, 0, 0, // Latitude
							0, 0, 0, 0, 0, 0, 0, 0, // Longitude
							0, 0, 0, 0, 0, 0, 0, 0, // Altitude
							0, 0, 0, 0, 0, 0, 0, 0, // Heading
							0, 0, 0, 0, 0, 0, 0, 0, // Bank
							0, 0, 0, 0, 0, 0, 0, 0, // Pitch
							};

		std::unique_ptr<AbstractCommandConfiguration> abstractCommand = CommandConfigurationParser::parse(rawBytes, 56);

		Assert::IsTrue(SET == abstractCommand->getCommand());

		SetIndicatorCommandConfiguration* setCommand = static_cast<SetIndicatorCommandConfiguration*>(abstractCommand.get());
		Assert::IsTrue(setCommand->getID() == 1);
		Assert::IsTrue(setCommand->getID() == 1);

		WorldPosition worldPos = setCommand->getPosition();
		Assert::IsTrue(worldPos.getLatitude() == 0.0);
		Assert::IsTrue(worldPos.getLongitude() == 0.0);
		Assert::IsTrue(worldPos.getAltitude() == 0.0);
		Assert::IsTrue(worldPos.getHeading() == 0.0);
		Assert::IsTrue(worldPos.getBank() == 0.0);
		Assert::IsTrue(worldPos.getPitch() == 0.0);
	}

	TEST_METHOD(TestSetCommandWithInvalidLatitudeHigh)
	{
		char rawBytes[] = { 0, 1,					// command
							0, 1,					// Indicator ID
							0, 0, 0, 1,				// Indicator Type ID
							0x40, 0x56, 0x80, 0x14, 0x7A, 0xE1, 0x47, 0xAE, // Latitude (90.00000001)
							0, 0, 0, 0, 0, 0, 0, 0, // Longitude
							0, 0, 0, 0, 0, 0, 0, 0, // Altitude
							0, 0, 0, 0, 0, 0, 0, 0, // Heading
							0, 0, 0, 0, 0, 0, 0, 0, // Bank
							0, 0, 0, 0, 0, 0, 0, 0, // Pitch
		};

		try
		{
			CommandConfigurationParser::parse(rawBytes, 56);
			Assert::Fail(L"Expected std::invalid_argument was not thrown");
		}
		catch (const std::invalid_argument e)
		{
			Assert::IsTrue(strcmp(e.what(), "LATITUDE_OUT_OF_RANGE") == 0);
		}
	}

	TEST_METHOD(TestSetCommandWithInvalidLatitudeLow)
	{
		char rawBytes[] = { 0, 1,					// command
							0, 1,					// Indicator ID
							0, 0, 0, 1,				// Indicator Type ID
							0xC0, 0x56, 0x80, 0x00, 0x00, 0x0A, 0xBC, 0xC7, // Latitude (-90.00000001)
							0, 0, 0, 0, 0, 0, 0, 0, // Longitude
							0, 0, 0, 0, 0, 0, 0, 0, // Altitude
							0, 0, 0, 0, 0, 0, 0, 0, // Heading
							0, 0, 0, 0, 0, 0, 0, 0, // Bank
							0, 0, 0, 0, 0, 0, 0, 0, // Pitch
		};

		try
		{
			CommandConfigurationParser::parse(rawBytes, 56);
			Assert::Fail(L"Expected std::invalid_argument was not thrown");
		}
		catch (const std::invalid_argument e)
		{
			Assert::IsTrue(strcmp(e.what(), "LATITUDE_OUT_OF_RANGE") == 0);
		}
	}

	TEST_METHOD(TestSetCommandWithInvalidLongitudeHigh)
	{
		char rawBytes[] = { 0, 1,					// command
							0, 1,					// Indicator ID
							0, 0, 0, 1,				// Indicator Type ID
							0, 0, 0, 0, 0, 0, 0, 0, // Latitude 
							0x40, 0x66, 0x80, 0x00, 0x00, 0x05, 0x5E, 0x64, // Longitude (180.00000001)
							0, 0, 0, 0, 0, 0, 0, 0, // Altitude
							0, 0, 0, 0, 0, 0, 0, 0, // Heading
							0, 0, 0, 0, 0, 0, 0, 0, // Bank
							0, 0, 0, 0, 0, 0, 0, 0, // Pitch
		};

		try
		{
			CommandConfigurationParser::parse(rawBytes, 56);
			Assert::Fail(L"Expected std::invalid_argument was not thrown");
		}
		catch (const std::invalid_argument e)
		{
			Assert::IsTrue(strcmp(e.what(), "LONGITUDE_OUT_OF_RANGE") == 0);
		}
	}

	TEST_METHOD(TestSetCommandWithInvalidLongitudeLow)
	{
		char rawBytes[] = { 0, 1,					// command
							0, 1,					// Indicator ID
							0, 0, 0, 1,				// Indicator Type ID
							0, 0, 0, 0, 0, 0, 0, 0, // Latitude
							0xC0, 0x66, 0x80, 0x00, 0x00, 0x05, 0x5E, 0x64, // Longitude (-180.00000001)
							0, 0, 0, 0, 0, 0, 0, 0, // Altitude
							0, 0, 0, 0, 0, 0, 0, 0, // Heading
							0, 0, 0, 0, 0, 0, 0, 0, // Bank
							0, 0, 0, 0, 0, 0, 0, 0, // Pitch
		};

		try
		{
			CommandConfigurationParser::parse(rawBytes, 56);
			Assert::Fail(L"Expected std::invalid_argument was not thrown");
		}
		catch (const std::invalid_argument e)
		{
			Assert::IsTrue(strcmp(e.what(), "LONGITUDE_OUT_OF_RANGE") == 0);
		}
	}
};
