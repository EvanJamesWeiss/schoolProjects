import javax.swing.*;
public class StackPopButton extends JButton {
    private Stack<SmartRectangle> _stack;
    private StackPanel _stackPanel;
    private LocationHolder _locationHolder;
    public StackPopButton(StackPanel aStackPanel, LocationHolder 						aHolder) {
        super("Pop");
        _stackPanel = aStackPanel;
        _stack = _stackPanel.getStack();
        _locationHolder = aHolder;
        this.addActionListener(new PopButtonListener());
    }
    private class PopButtonListener implements java.awt.event.ActionListener {
           public void actionPerformed(java.awt.event.ActionEvent e) {
               if (!_stack.isEmpty()) {
                   _stack.pop(); // remove item from stack
                   _locationHolder.incrementY();
                   _stackPanel.repaint();
               }
           }
       }
   }
