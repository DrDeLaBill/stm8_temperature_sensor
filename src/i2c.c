#include "i2c.h"

#include "stm8s.h"
#include "main.h"
#include "utils.h"


#define BUS_RESET_TIME_MS 10
#define I2C_DEFAULT_DELAY 5
#define I2C_WAIT_WHILE(condition, time) if (!wait_event(condition, time)) {delay_ms(BUS_RESET_TIME_MS); return I2C_TIMEOUT;}


bool _is_i2c_free();
bool _is_i2c_sb();
bool _is_i2c_addr();
bool _is_i2c_txe();
bool _is_i2c_btf();
bool _is_i2c_txe_and_btf();
bool _is_i2c_rxne();
bool _is_i2c_stop();


//******************************************************************************
// Инициализация I2C интерфейса      
// f_master_hz - частота тактирования периферии Fmaster          
// f_i2c_hz - скорость передачи данных по I2C             
//******************************************************************************                                   
void i2c_master_init(uint32_t f_master_hz, uint32_t f_i2c_hz)
{
	uint16_t ccr;

	GPIOB->DDR &= ~GPIO_PIN_4;
	GPIOB->DDR &= ~GPIO_PIN_5;
	GPIOB->ODR |= GPIO_PIN_4; //SCL
	GPIOB->ODR |= GPIO_PIN_5; //SDA

	GPIOB->CR1 &= ~GPIO_PIN_4;
	GPIOB->CR1 &= ~GPIO_PIN_5;

	GPIOB->CR2 &= ~GPIO_PIN_4;
	GPIOB->CR2 &= ~GPIO_PIN_5;

	//Частота тактирования периферии MHz
	I2C->FREQR = f_master_hz / 1000000;
	//Отключаем I2C
	I2C->CR1 &= ~I2C_CR1_PE;
	//В стандартном режиме скорость I2C max = 100 кбит/с
	//Выбираем стандартный режим
	I2C->CCRH &= ~I2C_CCRH_FS;
	//CCR = Fmaster/2*Fiic= 12MHz/2*100kHz
	ccr = f_master_hz / (2 * f_i2c_hz);
	//Set Maximum Rise Time: 1000ns max in Standard Mode
	//= [1000ns/(1/InputClockFrequencyMHz.10e6)]+1
	//= InputClockFrequencyMHz+1
	I2C->TRISER = (f_master_hz / 1000000) + 1;
	I2C->CCRL = ccr & 0xFF;
	I2C->CCRH = (ccr >> 8) & 0x0F;
	//Включаем I2C
	I2C->CR1 |= I2C_CR1_PE;
	//Разрешаем подтверждение в конце посылки
	I2C->CR2 |= I2C_CR2_ACK;
}

//******************************************************************************
// Запись регистра slave-устройства
//******************************************************************************                                   
i2c_status_t i2c_wr_reg(unsigned char address, unsigned char reg_addr, char *data, unsigned char length)
{                                                           
	//Ждем освобождения шины I2C
	I2C_WAIT_WHILE(&_is_i2c_free, 10);

	//Генерация СТАРТ-посылки
	I2C->CR2 |= I2C_CR2_START;
	//Ждем установки бита SB
	I2C_WAIT_WHILE(&_is_i2c_sb, I2C_DEFAULT_DELAY);


	//Записываем в регистр данных адрес ведомого устройства
	I2C->DR = address & 0xFE;
	//Ждем подтверждения передачи адреса
	I2C_WAIT_WHILE(&_is_i2c_addr, I2C_DEFAULT_DELAY);
	//Очистка бита ADDR чтением регистра SR3
	I2C->SR3;


	//Ждем освобождения регистра данных
	I2C_WAIT_WHILE(&_is_i2c_txe, I2C_DEFAULT_DELAY);
	//Отправляем адрес регистра
	I2C->DR = reg_addr;

	//Отправка данных
	while(length--){
		//Ждем освобождения регистра данных
		I2C_WAIT_WHILE(&_is_i2c_txe, I2C_DEFAULT_DELAY);
		//Отправляем адрес регистра
		I2C->DR = *data++;
	}

	//Ловим момент, когда DR освободился и данные попали в сдвиговый регистр
	I2C_WAIT_WHILE(&_is_i2c_txe_and_btf, I2C_DEFAULT_DELAY);

	//Посылаем СТОП-посылку
	I2C->CR2 |= I2C_CR2_STOP;
	//Ждем выполнения условия СТОП
	I2C_WAIT_WHILE(&_is_i2c_stop, I2C_DEFAULT_DELAY);

	return I2C_SUCCESS;
}

//******************************************************************************
// Чтение регистра slave-устройства
// Start -> Slave Addr -> Reg. addr -> Restart -> Slave Addr <- data ... -> Stop 
//******************************************************************************                                   
i2c_status_t i2c_rd_reg(uint8_t address, uint8_t reg_addr, uint8_t* data, uint8_t length)
{
	//Ждем освобождения шины I2C
	I2C_WAIT_WHILE(&_is_i2c_free, 10);

	//Разрешаем подтверждение в конце посылки
	I2C->CR2 |= I2C_CR2_ACK;

	//Генерация СТАРТ-посылки
	I2C->CR2 |= I2C_CR2_START;
	//Ждем установки бита SB
	I2C_WAIT_WHILE(&_is_i2c_sb, I2C_DEFAULT_DELAY);

	//Записываем в регистр данных адрес ведомого устройства
	I2C->DR = address & 0xFE;
	//Ждем подтверждения передачи адреса
	I2C_WAIT_WHILE(&_is_i2c_addr, I2C_DEFAULT_DELAY);
	//Очистка бита ADDR чтением регистра SR3
	I2C->SR3;

	//Ждем освобождения регистра данных RD
	I2C_WAIT_WHILE(&_is_i2c_txe, I2C_DEFAULT_DELAY);

	//Передаем адрес регистра slave-устройства, который хотим прочитать
	I2C->DR = reg_addr;
	//Ловим момент, когда DR освободился и данные попали в сдвиговый регистр
	I2C_WAIT_WHILE(&_is_i2c_txe_and_btf, I2C_DEFAULT_DELAY);

	//Генерация СТАРТ-посылки (рестарт)
	I2C->CR2 |= I2C_CR2_START;
	//Ждем установки бита SB
	I2C_WAIT_WHILE(&_is_i2c_sb, I2C_DEFAULT_DELAY);

	//Записываем в регистр данных адрес ведомого устройства и переходим
	//в режим чтения (установкой младшего бита в 1)
	I2C->DR = address | 0x01;

	//Дальше алгоритм зависит от количества принимаемых байт
	//N=1
	if (length == 1) {
		//Запрещаем подтверждение в конце посылки
		I2C->CR2 &= ~I2C_CR2_ACK;
		//Ждем подтверждения передачи адреса
		I2C_WAIT_WHILE(&_is_i2c_addr, I2C_DEFAULT_DELAY);

		//Заплатка из Errata
		disableInterrupts();
		//Очистка бита ADDR чтением регистра SR3
		I2C->SR3;

		//Устанавлием бит STOP
		I2C->CR2 |= I2C_CR2_STOP;
		//Заплатка из Errata
		enableInterrupts();

		//Ждем прихода данных в RD
		I2C_WAIT_WHILE(&_is_i2c_rxne, I2C_DEFAULT_DELAY);

		//Читаем принятый байт
		*data = I2C->DR;
	} else if(length == 2) { //N=2
		//Бит который разрешает NACK на следующем принятом байте
		I2C->CR2 |= I2C_CR2_POS;
		//Ждем подтверждения передачи адреса
		I2C_WAIT_WHILE(&_is_i2c_addr, I2C_DEFAULT_DELAY);
		//Заплатка из Errata
		disableInterrupts();
		//Очистка бита ADDR чтением регистра SR3
		I2C->SR3;
		//Запрещаем подтверждение в конце посылки
		I2C->CR2 &= ~I2C_CR2_ACK;
		//Заплатка из Errata
		enableInterrupts();
		//Ждем момента, когда первый байт окажется в DR,
		//а второй в сдвиговом регистре
		I2C_WAIT_WHILE(&_is_i2c_btf, I2C_DEFAULT_DELAY);

		//Заплатка из Errata
		disableInterrupts();
		//Устанавлием бит STOP
		I2C->CR2 |= I2C_CR2_STOP;
		//Читаем принятые байты
		*data++ = I2C->DR;
		//Заплатка из Errata
		enableInterrupts();
		*data = I2C->DR;
	} else if(length > 2) { //N>2
		//Ждем подтверждения передачи адреса
		I2C_WAIT_WHILE(&_is_i2c_addr, I2C_DEFAULT_DELAY);

		//Заплатка из Errata
		disableInterrupts();

		//Очистка бита ADDR чтением регистра SR3
		I2C->SR3;

		//Заплатка из Errata
		enableInterrupts();

		while(length-- > 3){
			//Ожидаем появления данных в DR и сдвиговом регистре
			I2C_WAIT_WHILE(&_is_i2c_btf, I2C_DEFAULT_DELAY);
			//Читаем принятый байт из DR
			*data++ = I2C->DR;
		}

		//Осталось принять 3 последних байта
		//Ждем, когда в DR окажется N-2 байт, а в сдвиговом регистре
		//окажется N-1 байт
		I2C_WAIT_WHILE(&_is_i2c_btf, I2C_DEFAULT_DELAY);
		//Запрещаем подтверждение в конце посылки
		I2C->CR2 &= ~I2C_CR2_ACK;
		//Заплатка из Errata
		disableInterrupts();
		//Читаем N-2 байт из RD, тем самым позволяя принять в сдвиговый
		//регистр байт N, но теперь в конце приема отправится посылка NACK
		*data++ = I2C->DR;
		//Посылка STOP
		I2C->CR2 |= I2C_CR2_STOP;
		//Читаем N-1 байт
		*data++ = I2C->DR;
		//Заплатка из Errata
		enableInterrupts();
		//Ждем, когда N-й байт попадет в DR из сдвигового регистра
		I2C_WAIT_WHILE(&_is_i2c_rxne, I2C_DEFAULT_DELAY);
		//Читаем N байт
		*data++ = I2C->DR;
	}

	//Ждем отправки СТОП посылки
	I2C_WAIT_WHILE(&_is_i2c_stop, I2C_DEFAULT_DELAY);
	//Сбрасывает бит POS, если вдруг он был установлен
	I2C->CR2 &= ~I2C_CR2_POS;

	return I2C_SUCCESS;
}

bool _is_i2c_free()
{
	return !(I2C->SR3 & I2C_SR3_BUSY);
}

bool _is_i2c_sb()
{
	return I2C->SR1 & I2C_SR1_SB;
}

bool _is_i2c_addr()
{
	return I2C->SR1 & I2C_SR1_ADDR;
}

bool _is_i2c_txe()
{
	return I2C->SR1 & I2C_SR1_TXE;
}

bool _is_i2c_btf()
{
	return I2C->SR1 & I2C_SR1_BTF;
}


bool _is_i2c_txe_and_btf()
{
	return _is_i2c_txe() && _is_i2c_btf();
}

bool _is_i2c_rxne()
{
	return I2C->SR1 & I2C_SR1_RXNE;
}

bool _is_i2c_stop()
{
	return !(I2C->CR2 & I2C_CR2_STOP);
}