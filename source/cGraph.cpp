#include "cGraph.h"
#include <QDebug>
#include "global.h"

/* 0 0 0 0 +
 * 0 0 0 0 +
 * 0 0 0 0 +
 * 0 0 0 0 +
 * + + + + +   <- **graph
 *         ^
 *      *graph
 */

void cGraph::SetLength(unsigned int tLength)
{
    if (tLength > length)
    {
        cGraph temp(tLength);

        for (unsigned int i = 0; i < tLength; i++)
            for (unsigned int j = 0; j < tLength; j++)
                if ((i < length) && (j < length))
                    temp.SetValue(graph[i][j],i,j);
                else
                    temp.SetValue(0.0f,i,j);

        Match(temp);
    }

}

void cGraph::Match (cGraph &temp)
{
    for (unsigned int i = 0; i<length; i++)
        delete []graph[i];

    length = temp.GetLength();

    graph = new float *[length];
    for (unsigned int i = 0; i < length; i++)
        graph[i] = new float[length];

    for (unsigned int i = 0; i < length; i++)
        for (unsigned int j = 0; j < length; j++)
            graph[i][j] = temp.GetValue(i,j);
}

void cGraph::Debug_ShowInfo()
{
    qDebug () << "cGraph Debug_ShowInfo";
    qDebug () << "Length: "<< GetLength();
    for (unsigned int i = 0; i < length; i++)
        for (unsigned int j = 0; j < length; j++)
                qDebug() << "position: " << i << j << "||"  << "value" << GetValue(i,j) << "RealID: " << i;
    qDebug() << "==========================================";
}

void cGraph::ShowDecayChain()
{
    cIsotope* pParent;
    cIsotope* pDaughter;

    for (int i = 0; i <= chain.GetIsotopeCount(); i++ )
        for (int j = 0; j <=chain.GetIsotopeCount(); j++)
            if (graph[i][j] != 0)
            {
                pParent = chain.GetElement(i);
                pDaughter = chain.GetElement(j);
                qDebug() << pParent->GetIsotopeMass() << pParent->GetIsotopeName() << "->"
                         << pDaughter->GetIsotopeMass() << pDaughter->GetIsotopeName() << "lambda = " << pParent->GetLambda() << "probability =" <<graph[i][j];
            }
}
