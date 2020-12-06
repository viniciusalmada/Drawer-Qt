#ifndef BOX_H
#define BOX_H

#include <cmath>

namespace RectUtils {
	
	class RectF {
		qreal left;
		qreal right;
		qreal bot;
		qreal top;
	public:
		RectF(double xp, double yp, double w, double h) {
			this->left = xp;
			this->right = xp + w;
			this->bot = yp;
			this->top = yp + h;
		}
		
		explicit RectF(QRectF rect = QRectF{}) {
			this->left = rect.left() < rect.right() ? rect.left() : rect.right();
			this->right = rect.left() > rect.right() ? rect.left() : rect.right();
			this->bot = rect.bottom() < rect.top() ? rect.bottom() : rect.top();
			this->top = rect.bottom() > rect.top() ? rect.bottom() : rect.top();
		}
		
		void update(const RectF& box) {
			if (box.left < left)
				setLeft(box.left);
			
			if (box.right > right)
				setRight(box.right);
			
			if (box.bot < bot)
				setBot(box.bot);
			
			if (box.top > top)
				setTop(box.top);
			
		}
		
		bool contains(const RectF& box) const {
			return box.left >= left &&
			       box.right <= right &&
			       box.bot >= bot &&
			       box.top <= top;
		}
		
		qreal getLeft() const {
			return left;
		}
		
		qreal getRight() const {
			return right;
		}
		
		qreal getBot() const {
			return bot;
		}
		
		qreal getTop() const {
			return top;
		}
		
		qreal getWidth() const {
			return right - left;
		}
		
		qreal getHeight() const {
			return top - bot;
		}
		
		qreal centerX() const {
			return left + (right - left) / 2.0;
		}
		
		qreal centerY() const {
			return bot + (top - bot) / 2.0;
		}
		
		void setLeft(double d) {
			left = d;
		}
		
		void setBot(double d) {
			bot = d;
		}
		
		void setRight(double d) {
			right = d;
		}
		
		void setTop(double d) {
			top = d;
		}
	};
};

#endif