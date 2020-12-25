#include "odesolver.h"
#include "stdio.h"
#include "boost/numeric/odeint.hpp"
#include <QDebug>
#include <QTime>

using namespace boost::numeric::odeint;
using namespace std;

typedef boost::numeric::ublas::vector<double> tVector;
typedef boost::numeric::ublas::matrix<double> tMatrix;

struct stiff_system
{
    void operator() (const tVector &N, tVector &dNdt, const double)
{
    unsigned int len = graph.GetLength();
    cIsotope *pPar;
    cIsotope *pDau;

    for (unsigned int i = 0; i < len; i++)
    {
        double add = 0.0;


        for  (unsigned int j = 0; j < len; j++)
            //if ((i != j)&&(graph.GetValue(j,i) != 0))
            {
                pDau = chain.GetElement(j);
                add += pDau->GetLambda() * N[j] * graph.GetValue(j,i);
            }

        pPar = chain.GetElement(i);
        dNdt[i] = -(pPar->GetLambda())* N[i] + add;
    }
}
};

struct stiff_system_jacobi
{
    void operator() (const tVector &, tMatrix &J, const double &, tVector &dNdt)
    {
        for (unsigned int i = 0; i < graph.GetLength(); i++)
            for (unsigned int j = 0; j < graph.GetLength(); j++)
                if (i == j)
                {
                   cIsotope *pIso = chain.GetElement(i);
                   J(i,j) = -( pIso->GetLambda() );
                }
                else
                {
                    cIsotope *pIso = chain.GetElement(j);
                    J(i,j) = -( pIso->GetLambda() ) * graph.GetValue(j,i);
                }
        for (unsigned int i = 0; i < graph.GetLength(); i++)
            dNdt[i] = 0.0;
    }
};

struct ODE_OUT
{
    QTextStream &out1, &out2;

    ODE_OUT (QTextStream &stream1,QTextStream &stream2) : out1 (stream1), out2 (stream2) {}

    template <class State >
    void operator() (const State &N, double t) const
    {
        std::cout <<'\r' <<"Time: " << t << " Done: " << t/MaxTime*100 << '%';
        out1 << t << '\t';
        out2 << t << '\t';
        for (unsigned int i = 0; i < graph.GetLength(); i++)
            if (N[i] <0)
            {
                out1 << 0 << '\t';
                out2 << t << '\t';
            }
            else
            {
                cIsotope *pIso = chain.GetElement(i);
                out1 << N[i]/pIso->GetParticleMass()*pIso->GetLambda() << '\t';
                out2 << N[i] << '\t';
            }
        out1 << endl;
        out2 << endl;
    }
};

void ODE_SOLV (double init, double end, double step, double ABS, double REL, QTextStream& stream1,QTextStream& stream2)
{
    tVector x;
    x.resize(graph.GetLength());
    for (unsigned int i = 0; i < graph.GetLength(); i++)
    {
        cIsotope *pIso = chain.GetElement(i);
        x[i] = pIso->GetMass();
    }

    QTime time;
    time.start ();

    size_t num_of_steps = integrate_const( make_dense_output< rosenbrock4 < double > >( ABS , REL ),
                make_pair ( stiff_system() , stiff_system_jacobi() ),
                x, init, end , step, ODE_OUT(stream1, stream2));

    qDebug() <<'\n' << "time elapsed: " << time.elapsed()/1000.0f << "sec";
    qDebug() << " number of steps: "<<num_of_steps << endl;
}
