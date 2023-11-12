//https://community.dynamics.com/blogs/post/?postid=7f554024-e9a4-4346-876b-3e72d42b1a2a
//https://winthropdc.wordpress.com/2017/07/24/ev3basic-using-the-mindsensors-sensor-multiplexer-with-ev3-basic/
//https://gist.github.com/basicxman/959353

#pragma config(Sensor, dgtl1,  sda,              sensorDigitalOut)
#pragma config(Sensor, dgtl2,  scl,              sensorDigitalOut)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define I2C_DELAY_COUNT 300
#define READ  false
#define WRITE true

void i2c_delay() {
  for (int i = 0; i < I2C_DELAY_COUNT; ++i) {}
}

void i2c_set_lines(bool direction) {
  SensorType[sda] = (direction) ? sensorDigitalOut : sensorDigitalIn;
  SensorType[scl] = (direction) ? sensorDigitalOut : sensorDigitalIn;
}

void i2c_start() {
  i2c_set_lines(WRITE);
  SensorValue[sda] = 0;
  SensorValue[scl] = 1;
  i2c_delay();
  SensorValue[sda] = 1;
  SensorValue[scl] = 0;
}

void i2c_stop() {
  i2c_set_lines(WRITE);
  SensorValue[sda] = 1;
  SensorValue[scl] = 1;
  i2c_delay();
}

void i2c_send_bit(bool bit) {
  i2c_set_lines(WRITE);
  SensorValue[sda] = bit;
  SensorValue[scl] = 1;
  i2c_delay();
  SensorValue[scl] = 0;
}

void i2c_send_byte(unsigned char byte_to_send, bool should_send_start = false, bool should_send_stop = false) {
  if (should_send_start) {
    i2c_start();
  }

  unsigned char temp_byte = byte_to_send;
  for (int b = 0; b < 8; ++b) {
    /*
     * 0x80 is 10000000 in binary, here a bit mask is used to set
     * every bit besides the MSB to 0.  If the MSB is 1 then the &
     * logic will set our temporary bit to 1, otherwise 0.  The
     * temp byte is then shifted left to make the next bit the MSB.
     */
    bool bit = (temp_byte & 0x80);
    temp_byte <<= 1;
    i2c_send_bit(bit);
  }

  if (should_send_stop) {
    i2c_stop();
  }
}

bool i2c_read_bit() {
  i2c_set_lines(READ);
  int timeout = time1[T1] + 5; // 5mS timeout.
  while (SensorValue[scl] == 0 || time1[T1] > timeout); // Wait for clock to become high.
  return (bool)SensorValue[sda];
}

unsigned char i2c_read_byte(bool should_send_stop = false) {
  unsigned char temp_byte = 0;
  for (int b = 0; b < 8; ++b) {
    /*
     * The i2c protocol is specified as MSB first so the temp byte
     * is always shifted left.  The LSB is essentially ORed with
     * the read bit.
     */
    temp_byte |=  i2c_read_bit();
    temp_byte <<= 1;
  }

  if (should_send_stop) {
    i2c_stop();
  }
  return temp_byte;
}

void i2c_send_string(string value) {
  bool i2c_start;
  bool i2c_stop;

  for (int idx = 0; idx < strlen(value); ++idx) {
    i2c_start = (idx == 0);
    i2c_stop  = (idx == strlen(value) - 1);
    i2c_send_byte(strIndex(value, idx), i2c_start, i2c_stop);
  }
}

task main() {
  i2c_send_string("Testing!");
}
