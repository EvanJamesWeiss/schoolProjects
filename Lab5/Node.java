public class Node <ElementType>{
	private ElementType _data;
 	private Node<ElementType> _next;
	public Node(ElementType anItem,
			   Node<ElementType> aNode)
	{
		_data = anItem;
		_next = aNode;
	}
	public Node(ElementType anItem)
	{
		this(anItem, null);
	}
  public ElementType getData()
  {
    return _data;
  }
  public Node<ElementType> getNext()
  {
    return _next;
  }
  public void setNext(Node<ElementType> aNode)
  {
    _next = aNode;
  }
}
