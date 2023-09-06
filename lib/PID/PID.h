class PID {
private:
    int Kp, Ki, Kd;
    int P, I, D;
    int error = 0, cumError = 0, prevError = 0;
public:
    PID(int Kp, int Ki, int Kd) {
        this->Kp = Kp;
        this->Ki = Ki;
        this->Kd = Kd;
    }

    int output(int error) {
        this->prevError = this->error;
        this->error = error;
        // this->cumError += error;

        this->P = this->error * this->Kp;
        this->I = this->cumError * this->Ki;
        this->D = (this->error - this->prevError) * this->Kd;

        return P + I + D;
    }
};