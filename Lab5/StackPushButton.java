import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
public class StackPushButton extends JButton {
    private Stack <SmartRectangle> _stack;
    private StackPanel _stackPanel;
    private LocationHolder _locationHolder;
    private final int SQUARE_SIZE = 20;

    public StackPushButton(StackPanel aStackPanel, LocationHolder aHolder) {
            super("Push");
            _stackPanel = aStackPanel;
            _stack = _stackPanel.getStack();
            _locationHolder = aHolder;
            this.addActionListener(new PushButtonListener());
        }
    private class ChangeColor {
      	private Color _myColor;
      	private ChangeColor _nextColor;
          public ChangeColor(Color c, ChangeColor nextc) {
          	_myColor = c;
              _nextColor = nextc;
          }
          public ChangeColor(Color c) {
              _myColor = c;
              _nextColor = null;
          }
          public void setNextColor(ChangeColor cc){
                    _nextColor = cc;
                }
                public Color getColor() {
                	return _myColor;
                }
                public ChangeColor getNextColor(){
                	return _nextColor;
                }
            }
            private class PushButtonListener implements ActionListener {
               	private ChangeColor _currentColor;
                     private ChangeColor _blueChangeColor;
           		private ChangeColor _redChangeColor;
           	      private ChangeColor _greenChangeColor;
           	      private ChangeColor _yellowChangeColor;
           	      private ChangeColor _blackChangeColor;
                  public PushButtonListener() {
                  	_blackChangeColor = new ChangeColor(Color.BLACK);
                  	_yellowChangeColor = new ChangeColor(Color.YELLOW,
                  			_blackChangeColor);
                  	_greenChangeColor = new ChangeColor(Color.GREEN,
                  			_yellowChangeColor);
                  	_redChangeColor = new ChangeColor(Color.RED,
                  			_greenChangeColor);
                  	_blueChangeColor = new ChangeColor(Color.BLUE,
                  			_redChangeColor);
                  	_blackChangeColor.setNextColor(_blueChangeColor);
                  	_currentColor = _blueChangeColor;
                  }
                  public void actionPerformed(ActionEvent e) {
                		int currentX = _locationHolder.getX();
                		int currentY = _locationHolder.getY();
                		 _stack.push(new SmartRectangle(currentX, currentY, 	SQUARE_SIZE, SQUARE_SIZE, _currentColor.getColor(), 	_stackPanel));
                		this.changeColor();
                		_locationHolder.decrementY();
                		_stackPanel.repaint();
                	}
                	private void changeColor () {
                	       _currentColor = _currentColor.getNextColor();
                	}
                  }
                }        
