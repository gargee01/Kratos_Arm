#include <Wire.h>
#include <MPU6050.h>

#include <ros.h>
#include <std_msgs/Float32MultiArray.h>

MPU6050 mpu;
ros::NodeHandle nh;

std_msgs::Float32MultiArray arr;
ros::Publisher chatter("angles", &arr);


// Variables to hold sensor data
int16_t ax, ay, az;
int16_t gx, gy, gz;

// Variables for Roll, Pitch, and Yaw
float roll, pitch, yaw;

// Kalman filter variables
float P[2][2] = {{1, 0}, {0, 1}};  // Error covariance matrix
float Q_angle = 0.001;              // Process noise (initial value)
float Q_gyro = 0.003;               // Gyro process noise (initial value)
float R_angle = 0.03;               // Measurement noise
float K[2];                         // Kalman gain
float y, S;                         // Kalman variables
float dt;                           // Delta time

// Kalman filter state
float angle = 0;                    // Angle estimate (filtered yaw)
float bias = 0;                     // Gyroscope bias

// Timing variables
float previousTime = 0, currentTime;

// Advanced Kalman Filter dynamic adjustment variables
float gyro_noise_scaling_factor = 1.0;  // Scaling factor for gyro noise
float accel_error_threshold = 0.1;      // Threshold for accelerometer error compensation

void setup() {
  // Initialize Serial for debugging
  //Serial.begin(115200);

  // Initialize MPU6050
  Wire.begin();
  mpu.initialize();

  // Test connection to MPU6050

  // Initialize timing for Kalman filter
  previousTime = millis();
  nh.initNode();
  arr.data_length = 3;
  arr.data = (float*)malloc(sizeof(float) * arr.data_length);
  
  nh.advertise(chatter);
}

void loop() {
  // Get current time for delta time calculation
  currentTime = millis();
  dt = (currentTime - previousTime) / 1000.0;  // Convert to seconds
  previousTime = currentTime;

  // Get motion data from MPU6050
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Convert accelerometer values to 'g'
  float ax_g = ax / 16384.0;
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;

  // Convert gyroscope values to degrees per second
  float gx_dps = gx / 131.0;
  float gz_dps = gz / 131.0;

  // Calculate Roll and Pitch from accelerometer data
  roll = atan2(ay_g, az_g) * 180.0 / PI;
  pitch = atan2(-ax_g, sqrt(ay_g * ay_g + az_g * az_g)) * 180.0 / PI;

  // Dynamically adjust process noise based on sensor activity (Advanced Kalman Filter logic)
  if (abs(gx_dps) > accel_error_threshold || abs(gz_dps) > accel_error_threshold) {
    // Increase process noise when motion is detected
    Q_gyro = 0.01 * gyro_noise_scaling_factor;
    Q_angle = 0.005 * gyro_noise_scaling_factor;
  } else {
    // Reduce process noise when the sensor is stationary
    Q_gyro = 0.003 * gyro_noise_scaling_factor;
    Q_angle = 0.001 * gyro_noise_scaling_factor;
  }

  // Predict yaw using the gyroscope (Kalman filter prediction step)
  yaw = advancedKalmanFilter(gz_dps, roll, dt);

  // Print Roll, Pitch, and Yaw (Filtered Yaw using Advanced Kalman Filter)
  arr.data[0] = roll;
  arr.data[1] = pitch;
  arr.data[2] = yaw;
  // Small delay to control output rate
  delay(100);
  chatter.publish(&arr);
  nh.spinOnce();
}

// Advanced Kalman filter function for yaw estimation
float advancedKalmanFilter(float gyro_rate, float accel_angle, float dt) {
  // Step 1: Prediction (predict the next angle and error covariance)
  angle += dt * (gyro_rate - bias);  // Update angle estimate using the gyro rate
  P[0][0] += dt * (dt*P[1][1] - P[0][1] - P[1][0] + Q_angle);
  P[0][1] -= dt * P[1][1];
  P[1][0] -= dt * P[1][1];
  P[1][1] += Q_gyro * dt;

  // Step 2: Correction (use accelerometer angle to correct)
  y = accel_angle - angle;  // Error between measured and predicted angle
  S = P[0][0] + R_angle;    // Estimate error

  // Compute Kalman gain
  K[0] = P[0][0] / S;
  K[1] = P[1][0] / S;

  // Update angle and bias estimates
  angle += K[0] * y;
  bias += K[1] * y;

  // Update error covariance matrix
  P[0][0] -= K[0] * P[0][0];
  P[0][1] -= K[0] * P[0][1];
  P[1][0] -= K[1] * P[0][0];
  P[1][1] -= K[1] * P[0][1];

  


  return angle;  // Return the filtered yaw angle
}
