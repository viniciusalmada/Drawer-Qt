#ifndef DRAWERQTPAINTER_BOX_H
#define DRAWERQTPAINTER_BOX_H

#include <cmath>

namespace RectUtils {
	
	static void update(QRectF& main, const QRectF& box) {
		main.setLeft(main.left() < box.left() ? box.left() : main.left());
		main.setRight(main.right() > box.right() ? box.right() : main.right());
		main.setBottom(main.bottom() < box.bottom() ? box.bottom() : main.bottom());
		main.setTop(main.top() > box.top() ? box.top() : main.top());
	}
	
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
			setLeft(left < box.left ? box.left : left);
			setRight(right > box.right ? box.right : right);
			setBot(bot < box.bot ? box.bot : bot);
			setTop(top > box.top ? box.top : top);
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
		
		double maxX() const {
			return right;
		}
		
		double maxY() const {
			return top;
		}
	};
};

#endif