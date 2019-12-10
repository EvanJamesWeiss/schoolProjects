public class LocationHolder {
	private int _x, _y;
	private final int SQUARE_SIZE = 20;
	private final int STACK_X = 275;
	private final int MAX_Y = 350;
	public LocationHolder () {
		_x = STACK_X; // this won't change
		_y = MAX_Y; // to start
	}

  public int getX() {
		return _x;
	}
	public int getY() {
		return _y;
	}
	public void decrementY() {
		_y -= (SQUARE_SIZE+5);
	}
	public void incrementY() {
		_y += (SQUARE_SIZE+5);
	}
}
