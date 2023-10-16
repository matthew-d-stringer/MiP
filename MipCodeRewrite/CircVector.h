class CircVector {
private:
    float *vector;
    int size;
    int cIndex = 0;
public:
    CircVector(int size) {
        this->size = size;
        vector = new float[size];
        reset();
    }
    void reset() {
        for(int i = 0; i < size; i++)
            vector[i] = 0;
        cIndex = 0;
    }

    float & operator[] (int index);

    void increment() {
        cIndex--;
    }

private:
    int calcIndex(int i){
        return (i + cIndex) % size;
    }
};

float & CircVector::operator[](int index) {
    return vector[calcIndex(index)];
}