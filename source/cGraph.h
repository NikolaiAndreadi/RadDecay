#ifndef CGRAPH_H
#define CGRAPH_H


class cGraph
{
    float **graph;

    unsigned int length;

    void Match (cGraph &temp);

public:

    cGraph(int N)
    {
        graph = new float *[N];
        for (int i = 0; i < N; i++)
            graph[i] = new float[N];

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                graph[i][j] = 0.0f;
        length = N;
    }

    cGraph()
    {cGraph(2);}


    ~cGraph()
    {
        for (unsigned int i = 0; i<length; i++)
            delete []graph[i];
    }

    inline unsigned int GetLength () {return length;}
    inline float GetValue (int i, int j) {return graph[i][j];}
    inline void SetValue (float value, int i, int j ) {graph[i][j] = value;}
    void SetLength (unsigned int tLength);
    void Debug_ShowInfo();
    void ShowDecayChain();
};

#endif // CGRAPH_H
