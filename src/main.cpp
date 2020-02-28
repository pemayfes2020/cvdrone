#include "ardrone/ardrone.h"

// --------------------------------------------------------------------------
// main(Number of arguments, Argument values)
// Description  : This is the entry point of the program.
// Return value : SUCCESS:0  ERROR:-1
// --------------------------------------------------------------------------
const int START = 0;
const int FIND_OBJECT = 1;
const int GO_TOWARDS = 2;
const int ARRIVED = 3;


//中心座標確認用
void drawCirlcles(std::vector<cv::Vec3f> circles);

int main(int argc, char *argv[])
{
	// AR.Drone class
	ARDrone ardrone;

	// Initialize
	if (!ardrone.open()) {
		std::cout << "Failed to initialize." << std::endl;
		return -1;
	}

	// Instructions
	std::cout << "***************************************" << std::endl;
	std::cout << "*       CV Drone sample program       *" << std::endl;
	std::cout << "* - Controls -                        *" << std::endl;
	std::cout << "*    'Space' -- Takeoff/Landing       *" << std::endl;
	std::cout << "*    'Up'    -- Move forward          *" << std::endl;
	std::cout << "*    'Down'  -- Move backward         *" << std::endl;
	std::cout << "*    'Left'  -- Turn left             *" << std::endl;
	std::cout << "*    'Right' -- Turn right            *" << std::endl;
	std::cout << "*    'Q'     -- Move upward           *" << std::endl;
	std::cout << "*    'A'     -- Move downward         *" << std::endl;
	std::cout << "*    'B'     -- auto-up-and-down      *" << std::endl;
	std::cout << "* - Others -                          *" << std::endl;
	std::cout << "*    'C'     -- Change camera         *" << std::endl;
	std::cout << "*    'Esc'   -- Exit                  *" << std::endl;
	std::cout << "***************************************" << std::endl;

	// Battery
	std::cout << "Battery = " << ardrone.getBatteryPercentage() << "[%]" << std::endl;


	double target_x, target_y, target_z;
	int phase = START;

	double pre_alt = 0;

	int LOW_HUE = 88;           //hueの下限
	int UP_HUE = 108;              //hueの上限

	while (1) {
		// Key input
		int key = cv::waitKey(32);
		if (key == 0x1b) break;

		// Get an image
		cv::Mat image = ardrone.getImage();

		//飛ぶと危ないのでコメントアウトしてる
		//Take off / Landing 
		if (key == ' ') {
			if (ardrone.onGround()) ardrone.takeoff();
			else ardrone.landing();
		}

		

		//phaseごとに分ける

		// switch (phase){
		// 	case START:{
		// 	    if(abs(pre_alt - ardrone.getAltitude()) < 0.5 && pre_alt){
		// 			phase = 1;//FIND_OBJECTへ
		// 		}
		// 		pre_alt = ardrone.getAltitude();
		// 		break;
		// 	}
		// 	case FIND_OBJECT:{
		// 		//その場で回転
		// 		vr = 1.0;

		// 		break;
		// 	}
		// 	case GO_TOWARDS:{
		// 		break;
		// 	}
		// 	case ARRIVED:{
		// 		break;
		// 	}
		// 	default:{
		// 		break;
		// 	}
		// }

		// Move
		double vx = 0.0, vy = 0.0, vz = 0.0, vr = 0.0;
		switch (key) {
		case CV_VK_UP:		vx = 1.0; break;
		case CV_VK_DOWN:	vx = -1.0; break;
		case CV_VK_LEFT:	vy = 1.0; break;
		case CV_VK_RIGHT:	vy = -1.0; break;
		case 'j':	vy = 1.0; break;
		case 'l':	vy = -1.0; break;
		case 'q':	vz = 1.0; break;
		case 'a':	vz = -1.0; break;
		case 'b':
			double vx_now, vy_now;
			double const init_alt = ardrone.getAltitude();

			std::cout << "altitude = " << init_alt << std::endl;
			while (ardrone.getAltitude() < init_alt + 0.3) {
				ardrone.getVelocity(&vx_now, &vy_now, 0);
				ardrone.move3D(-vx_now, -vy_now, 1.0, 0.0);
				std::cout << "\r" << "altitude = " << ardrone.getAltitude();
			}
			while (ardrone.getAltitude() > init_alt) {
				ardrone.getVelocity(&vx_now, &vy_now, 0);
				ardrone.move3D(-vx_now, -vy_now, -1.0, 0.0);
				std::cout << "\r" << "altitude = " << ardrone.getAltitude();
			}
			ardrone.move3D(0.0, 0.0, 1.0, 0.0);
			std::cout << "\r" << "command b end.            ";
			break;
		}
		ardrone.move3D(vx, vy, vz, vr);

		// Change camera
		static int mode = 0;
		if (key == 'c') ardrone.setCamera(++mode % 4);


		//標準出力で中心座標、半径を確認
		//drawCirlcles(ardrone.color_tracking(image, target_x, target_y, target_z));
	    drawCirlcles(ardrone.detectCircle(image, target_x, target_y, target_z, LOW_HUE, UP_HUE));



		
		//if (ardrone.getBatteryPercentage() < 10) {
		//	std::cout << "Battery low !" << std::endl;
		//	ardrone.landing();
		//	break;
		//}
	}

	// See you
	ardrone.close();

	return 0;
}

//検出した円の中心座標、半径を標準出力で確認する用
void drawCirlcles(std::vector<cv::Vec3f> circles){
	if(circles.size()){
			std::cout << std::endl;
			for(size_t i = 0;i < circles.size();i++){
				std::cout << "x = " << cvRound(circles[i][0]) << " y = " << cvRound(circles[i][1]) << std::endl;
				std::cout << "r = " << cvRound(circles[i][2]) << std::endl;
			}
			std::cout << std::endl;
		}
}
