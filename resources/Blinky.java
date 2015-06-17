public class Blinky
{
	static final int delay = 4;
	native static void setLeds (int state);
	
	public static void main(String[] args)
	{
		int n, k;
		for (n = 0; n < 8; n++)
			for (k = 0; k < delay; k++) setLeds (1 << n);
		for (n = 7; n >= 0; n--)
			for (k = 0; k < delay; k++) setLeds (1 << n);
	}
}
