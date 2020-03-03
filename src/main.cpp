#include "ardrone/ardrone.h"

// --------------------------------------------------------------------------
// main(Number of arguments, Argument values)
// Description  : This is the entry point of the program.
// Return value : SUCCESS:0  ERROR:-1
// --------------------------------------------------------------------------


int main(int argc, char *argv[])
{
	// AR.Drone class
	ARDrone ardrone;

	// Initialize
	if (!ardrone.open()) {
		std::cout << "Failed to initialize." << std::endl;
		return -1;
	}

	// Battery
	std::cout << "Battery = " << ardrone.getBatteryPercentage() << "[%]" << std::endl;

	// Instructions
	std::cout << "***************************************" << std::endl;
	std::cout << "*       CV Drone sample program       *" << std::endl;
	std::cout << "*           - How to play -           *" << std::endl;
	std::cout << "***************************************" << std::endl;
	std::cout << "*                                     *" << std::endl;
	std::cout << "* - Controls -                        *" << std::endl;
	std::cout << "*    'Space' -- Takeoff/Landing       *" << std::endl;
	std::cout << "*    'Up'    -- Move forward          *" << std::endl;
	std::cout << "*    'Down'  -- Move backward         *" << std::endl;
	std::cout << "*    'Left'  -- Move left             *" << std::endl;
	std::cout << "*    'Right' -- Move right            *" << std::endl;
	std::cout << "*    'A'     -- Turn left             *" << std::endl;
	std::cout << "*    'D'     -- Turn right            *" << std::endl;
	std::cout << "*    'W'     -- Move upward           *" << std::endl;
	std::cout << "*    'S'     -- Move downward         *" << std::endl;
	std::cout << "*    'R'     -- reset position        *" << std::endl;
	std::cout << "*                                     *" << std::endl;
	std::cout << "* - Others -                          *" << std::endl;
	std::cout << "*    'C'     -- Change camera         *" << std::endl;
	std::cout << "*    'Esc'   -- Exit                  *" << std::endl;
	std::cout << "*                                     *" << std::endl;
	std::cout << "***************************************" << std::endl;

	//–Ú•W’n“_‚ðŠi”[
	double target_x, target_y, target_z;
        target_x = 0.0; target_y = 0.0; target_z = 0.4444;

	while (1) {
		// Key input
		int key = cv::waitKey(5);
		if (key == 0x1b) break;

		// Get an image
		cv::Mat image = ardrone.getImage();

		// Take off / Landing 
		if (key == ' ') {
			if (ardrone.onGround()){
                            ardrone.takeoff();
                            while(ardrone.getAltitude() < 0.1){}
                            ardrone.mysetPosition(-1.0, -1.0, ardrone.getAltitude());
			}else{
                            ardrone.landing();
                        }
		}

		// Move
		double vx = 0.0, vy = 0.0, vz = 0.0, vr = 0.0;
                double vx_now = 0.0, vy_now = 0.0, vz_now,  x_now = 0.0, y_now = 0.0, z_now = 0.0;
		ardrone.getVelocity(&vx_now, &vy_now, &vz_now);	
		
		switch (key) {
		case CV_VK_UP:		target_y += 0.5; break;
		case CV_VK_DOWN:	target_y += -0.5; break;
		case CV_VK_LEFT:	target_x += 0.5; break;
		case CV_VK_RIGHT:	target_x += -0.5; break;
		case 'a':	vr = 1.0; break;
		case 'd':	vr = -1.0; break;
		case 'w':	vz = 1.0; break;
		case 's':	vz = -1.0; break;
		case 'r':	ardrone.resetPosition();

		default:
			ardrone.keepPosition(target_x, target_y, target_z, &vx, &vy, &vz);
			ardrone.mygetPosition(&x_now, &y_now, &z_now);
			std::cout << std::fixed;
                        std::cout << std::setprecision(3) << "" << "(" << x_now << ", " << y_now << ", " << z_now;
			std::cout << "), vel = (" << vx << ", " << vy << ", " << vz;
			std::cout << "), v_now = (" << vx_now << ", " << vy_now << ", " << vz_now;
                        std::cout << "), trg = (" << target_x << ", " << target_y << ", " << target_z;
		    std::cout << "), alt = " << ardrone.getAltitude() << std::endl;
            }
		ardrone.move3D(vx, vy, vz, vr);

		// Change camera
		static int mode = 0;
		if (key == 'c') ardrone.setCamera(++mode % 4);

		// Display the image
		cv::imshow("camera", image);

		if (ardrone.getBatteryPercentage() < 15) {
			std::cout << "Battery low !" << std::endl;
			ardrone.landing();
			break;
		}
	}

	// See you
	ardrone.close();

	return 0;
}
