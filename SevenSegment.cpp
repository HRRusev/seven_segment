/*
 * SevenSegment.cpp
 *
 *  Created on: May 15, 2025
 *      Author: eng. Hristian Rusev
 */

#include "SevenSegment.h"

SevenSegment::SevenSegment()
{

}

SevenSegment::SevenSegment(const segmentsPin (&pins)[8], const segmentDigit* digits, uint8_t segmentDigits_, bool commonCathode_)
:segmentPin(pins),					//struct with seven segments pins
digitsPin(digits), 				//struct with digits pins
numberDigits(segmentDigits_), 		//number of digits
commonCathode(commonCathode_)		//type of seven segment common cathode or common anode
{
	position = 0;
	bufferIndex = 0;
	off = false;
	symbolCodes = new char[numberDigits];
	buffer = new char[(2*numberDigits)+1];

}

void SevenSegment::intilization(const segmentsPin (&pins)[8], const segmentDigit *digits, uint8_t segmentDigits_, bool commonCathode_)
{
	segmentPin = pins;						//struct with seven segments pins
	digitsPin = digits; 					//struct with digits pins
	numberDigits = segmentDigits_; 			//number of digits
	commonCathode = commonCathode_;			//type of seven segment common cathode or common anode

	position = 0;
	bufferIndex = 0;
	off = false;
	symbolCodes = new char[numberDigits];
	buffer = new char[(2*numberDigits)+1];
}

bool SevenSegment::message(const char *str)
{
	uint8_t strIdx = 0;
	uint8_t symbolIdx = 0;
	while(this->buffer[strIdx] != '\n')
	{
		if(str[strIdx] >= '0' && str[strIdx] <= '9')
			this->symbolCodes[symbolIdx] = digitCodeMap[str[strIdx] - '0'];
		else if(str[strIdx] >= 'A' && str[strIdx] <= 'Z')
			this->symbolCodes[symbolIdx] = digitCodeMap[str[strIdx] - 'A' + 10];
		else if(str[strIdx] >= 'a' && str[strIdx] <= 'z')
			this->symbolCodes[symbolIdx] = digitCodeMap[str[strIdx] - 'a' + 10];
		else if (str[strIdx] == ' ')
			this->symbolCodes[symbolIdx] = digitCodeMap[BLANK_IDX];
		else if (str[strIdx] == '.')
			if(strIdx == 0)
				this->symbolCodes[symbolIdx] = digitCodeMap[DOT_IDX];
		else
		{
			this->symbolCodes[symbolIdx - 1] = digitCodeMap[DOT_IDX] | this->symbolCodes[symbolIdx - 1];
			--symbolIdx;
		}

		else if (str[strIdx] == '*')
			this->symbolCodes[symbolIdx] = digitCodeMap[ASTERISK_IDX];
		else if (str[strIdx] == '_')
			this->symbolCodes[symbolIdx] = digitCodeMap[UNDERSCORE_IDX];
		else  // Every unknown character is shown as a dash
			this->symbolCodes[symbolIdx] = digitCodeMap[DASH_IDX];


		++strIdx; ++symbolIdx;

	}

	return true;
}

uint8_t SevenSegment::intToString(int number)
{
	char temp[16];
	uint8_t i = 0;
	bool isNegative = number < 0;

	if (isNegative) { number = -number; }

	if (number == 0)
	{
		temp[i++] = '0';
	}
	else
	{
		while (number > 0) {
			temp[i++] = (number % 10) + '0';
			number /= 10;
		}
		if(isNegative) { temp[i++] = '-'; }
	}

	uint8_t k = 0;
	uint8_t digits = i;

	if(i != this->numberDigits)
	{
		uint8_t j = i;
		while(j < this->numberDigits)
		{
			this->buffer[k++] = ' ';
			++j;
		}

	}

	// Reverse digits into buffer
	while (i > 0) {
		this->buffer[k++] = temp[--i];
	}

	// Add newline
	this->buffer[k++] = '\n';

	return digits;

}

uint8_t SevenSegment::intToString(int number, char (&str)[16])
{
	char temp[16];
	uint8_t i = 0;
	bool isNegative = number < 0;

	if (isNegative) { number = -number; }

	if (number == 0)
	{
		temp[i++] = '0';
	}
	else
	{
		while (number > 0) {
			temp[i++] = (number % 10) + '0';
			number /= 10;
		}
		if(isNegative) { temp[i++] = '-'; }
	}

	// Reverse digits into buffer
	uint8_t k = 0;
	uint8_t digits = i;
	while (i > 0) {
		str[k++] = temp[--i];
	}

	return digits;
}

uint8_t SevenSegment::floatToString(float number)
{
	//	Return the number after the comma
	bool isNegative = number < 0;
	if (isNegative) { number = -number; }

	// Extract integer and fractional parts
	int integerPart = (int)number;
	float fractionalPart = number - (float)integerPart;

	if(isNegative) { integerPart = - integerPart; }
	char integerStr[16];
	uint8_t integerPlaces = this->intToString(integerPart, integerStr);
	uint8_t i = 0;
	//Display integer parts
	while(i < integerPlaces)
	{
		this->buffer[i] = integerStr[i];
		i++;
	}

	uint8_t fractionalDigits = 0;
	if(i < this->numberDigits)
	{
		int intFractionalPart = (int)(fractionalPart * powersOf10[this->numberDigits - integerPlaces]);
		char fractionalStr[16];
		this->intToString(intFractionalPart, fractionalStr);

		this->buffer[i++] = '.';
		while(i <= this->numberDigits)
		{
			this->buffer[i] = fractionalStr[fractionalDigits];
			i++; fractionalDigits++;
		}
	}

	// Add newline
	this->buffer[i] = '\n';

	return fractionalDigits;
}

bool SevenSegment::setNumber(int32_t number)
{
	int8_t member;
	if(number > 0)
		member = number / powersOf10[this->numberDigits - 1];
	else
		member = -(1 * number) / powersOf10[this->numberDigits - 2];
	if(member > 9 || member < -9)
	{
		//number is out of range on display
		setDash();
		return false;
	}

	this->intToString(number);
	this->message(this->buffer);


	return true;
}


int8_t SevenSegment::setNumberF(float numberF)
{
	// Return -1 for fauls or number after the comma
	uint8_t fractionalDigits = this->setNumberFloat(numberF);
	this->message(this->buffer);

	if(fractionalDigits == -1)
		return -1;

	return fractionalDigits;
}

bool SevenSegment::setNumberF_units(float numberF)
{
	int8_t fractionalDigits = this->setNumberFloat(numberF);
	if(fractionalDigits == -1)
	{
		this->setDash();
		return false;
	}


	if(fractionalDigits >  1)
	{
		uint8_t i = fractionalDigits - 1;
		char member[16];
		uint8_t pos = 0;
		//Add empty simbol
		while(i > 0)
		{
			member[pos] = ' ';
			--i; ++pos;
		}
		i = 0;
		//Get int part of number
		while(this->buffer[i] != '.')
		{
			member[pos] = buffer[i];
			++i; ++pos;
		}
		for(uint8_t k = 0; k < 2; ++k)
		{
			//Add . and digit;
			member[pos] = buffer[i];
			++i; ++pos;
		}
		member[pos] = '\n';
		//Copy result
		for(uint8_t k = 0; k <= pos; ++k)
			buffer[k] = member[k];
	}

	this->message(this->buffer);

	return true;
}
void SevenSegment::setOFF(bool displayOFF) { this->off = displayOFF; }
bool SevenSegment::getOFF() const { return this->off; }

void SevenSegment::setDash(void)
{
	for(uint8_t idx = 0; idx < this->numberDigits; ++idx)
		this->symbolCodes[idx] = digitCodeMap[DASH_IDX];
}


void SevenSegment::refreshDisplay(void) volatile
{

	this->segmentOFF();
	this->position++;
	if(this->position == this->numberDigits)
		this->position = 0;
	if(!this->off)
	{
		//Set segment outputs
		this->loadSymbol();
		this->segmentON();
	}

}

int8_t SevenSegment::setNumberFloat(float numberF)
{
	// Float to char buffer
	// Return -1 for fauls or number after the comma
	int8_t member;
	if(numberF >= 0.0)
		member = numberF / powersOf10[this->numberDigits - 1];
	else
		member = numberF / powersOf10[this->numberDigits - 2];

	if((member > 9 && numberF >= 0)||(member < -9 && numberF < 0.0))
	{
		//number is out of range on display
		this->setDash();
		return -1;
	}


	uint8_t fractionalDigits = this->floatToString(numberF);

	return fractionalDigits;
}


void SevenSegment::loadSymbol(void) volatile
{
	char symbol = *(this->symbolCodes + this->position);

	for(uint8_t i = 0; i < 8; ++i)
	{
		if((symbol & 0b00000001) == 0b00000001)
			HAL_GPIO_WritePin(this->segmentPin[i].port, this->segmentPin[i].pin, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(this->segmentPin[i].port, this->segmentPin[i].pin, GPIO_PIN_RESET);
		symbol >>= 1;
	}

}

void SevenSegment::segmentON(void) volatile
{
	uint8_t idx = this->numberDigits - 1 - this->position;
	if(this->commonCathode)
		HAL_GPIO_WritePin(this->digitsPin[idx].port, this->digitsPin[idx].pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(this->digitsPin[idx].port, this->digitsPin[idx].pin, GPIO_PIN_RESET);
}

void SevenSegment::segmentOFF(void) volatile
{
	uint8_t idx = this->numberDigits - 1 - this->position;
	if(this->commonCathode)
		HAL_GPIO_WritePin(this->digitsPin[idx].port, this->digitsPin[idx].pin, GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(this->digitsPin[idx].port, this->digitsPin[idx].pin, GPIO_PIN_SET);
}

void SevenSegment::operator<<(const char *str)
{
	char *member = new char[(2 * this->numberDigits) + 1];
	uint8_t size = sprintf(member, "%s", str) - 1;

	//Get number of points
	uint8_t pointNumber = 0;
	uint8_t symbolNumber = 0;
	uint8_t idx = 0;
	for(uint8_t i = 0; i < size; ++i)
	{
		if(member[i] == '.')
			++pointNumber;
		else
			++symbolNumber;
	}
	if(member[size -1 ] == '\n')
		--symbolNumber;
	//Size check
	if(symbolNumber > this->numberDigits)
	{
		for(uint8_t i = 0; i < this->numberDigits; ++i)
			this->buffer[i] = '-';
	}
	else if(symbolNumber < this->numberDigits)
	{
		for(; idx < numberDigits - symbolNumber; ++idx)
			this->buffer[idx] = ' ';

		uint8_t idxStr = 0;
		for(; idxStr < size; ++idxStr)
		{
			this->buffer[idx] = member[idxStr];
			++idx;
		}
	}
	else
	{
		sprintf(this->buffer, "%s", member);
		idx = size;
	}

	this->buffer[idx] = '\n';
	delete[] member;

	this->message(this->buffer);

}

SevenSegment::~SevenSegment()
{
	delete[] this->symbolCodes;
	delete[] this->buffer;
}

