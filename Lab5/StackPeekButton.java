import javax.swing.*;
import java.awt.event.*;

public class StackPeekButton extends JButton {
	private Stack<SmartRectangle> _stack;
	private StackPanel _stackPanel;
	private LocationHolder _locationHolder;
	public StackPeekButton(StackPanel aStackPanel, LocationHolder aHolder){
		super("Peek");
  	_stackPanel = aStackPanel;
  	_stack = _stackPanel.getStack();
  	_locationHolder = aHolder;
  	this.addActionListener(new PeekButtonListener());
  }
  private class PeekButtonListener implements ActionListener {
 		public void actionPerformed(ActionEvent e) {
      SmartRectangle tempRect, panelRect;
      if (!_stack.isEmpty()) {
 				tempRect = _stack.peek();
       	panelRect = _stackPanel.getDisplayRect();
        panelRect.setColor(tempRect.getColor());
        _stackPanel.repaint();
      }
 	  }
  }
 }
