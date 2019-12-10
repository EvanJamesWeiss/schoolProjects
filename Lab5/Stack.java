public interface Stack<ElementType>
{
  public void push (ElementType anItem);
  public ElementType pop();
  public ElementType peek();
  public boolean isEmpty();
}
