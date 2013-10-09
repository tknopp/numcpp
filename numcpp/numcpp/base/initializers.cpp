#include "initializers.h"
#include "../expressions.h"


namespace numcpp
{


Array<double> phantom(size_t N)
{
    Array<double> I = zeros(N,N);

    auto z = meshgrid<double,double>( linspace(-1,1,N) , (-1)*linspace(-1,1,N) );
    auto x = z.first;
    auto y = z.second;

    //auto s = 0.8;
    auto s = 0.4;

    I += 1.0 * ( pow(x/0.69,2.0)+pow(y/0.92,2.0) < 1);
    I += - s * ( pow(x/0.6624,2)+pow((y+0.0184)/0.874,2) < 1 );
    I += - 0.2 * ( pow( (cos(-18.0/360*2*pi)*(x-0.22)+sin(-18.0/360*2*pi)*y)/0.11,2) +
        pow( (sin(-18.0/360*2*pi)*(x-0.22)-cos(-18.0/360*2*pi)*y)/0.31,2) < 1 );
    I += - 0.2 * ( pow( (cos( 18.0/360*2*pi)*(x +0.22)+sin( 18.0/360*2*pi)*y)/0.16,2) +
        pow( (sin( 18.0/360*2*pi)*(x +0.22)-cos( 18.0/360*2*pi)*y)/0.41, 2) <1 );
    I += 0.1 * ( pow(x/0.21,2)+pow((y-0.35)/0.25,2) <1);
    I += 0.1 * ( pow(x/0.046,2)+pow((y-0.1)/0.046,2) <1 );
    I += 0.1 * ( pow(x/0.046,2)+pow((y+0.1)/0.046,2) <1 );
    I += 0.1 * ( pow((x+0.08)/0.046,2)+pow((y+0.605)/0.023,2) <1 );
    I += 0.1 * ( pow(x/0.023,2)+pow((y+0.606)/0.023,2) <1 );
    I += 0.1 * ( pow((x-0.06)/0.023,2)+pow((y+0.605)/0.046,2) <1 );

    return I;
}

}
