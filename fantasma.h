
class fantasma{
	protected:
		int _x, _y;
	public:
		virtual void dibujar_fantasma(void) = 0;
		virtual void chocar(void) = 0;
		virtual void fantasma_mover(void) = 0;
};
