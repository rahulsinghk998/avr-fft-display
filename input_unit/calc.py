import argparse
from math import cos
from math import pi
from math import floor

parser = argparse.ArgumentParser(description='Calculate the values of cos(2*pi*1/N)*M from 0..M')
parser.add_argument('N', type=int, help='an integer N for calculating the coefficient')
parser.add_argument('M', type=int, help='an integer M for multiplying the coefficient')

args = parser.parse_args()

out = '0'
for i in range(1, args.M):
    out += ', '
    out += str(int(floor(cos(2.*pi*1./float(args.N))*float(i)+.5)))
print out
