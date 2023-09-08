void Object::CheckHitToGrounds(const ML::Box2D& me_, void (*action_)(void*), void* arg_) {
	//�S�Ă̒n�`�Ɠ����蔻��
	for (auto& ground : (*grounds)) {
		if (ground->CheckHit(me_)) {
			(*action_)(arg_);
		}
	}
}

//y���̏���
while (est.y != 0.00f) {

	//���肷��O�̈ʒu���L��
	float preY = pos.y;

	if (est.y >= 1.00f) { ++me.y; ++pos.y; --est.y; }
	else if (est.y <= -1.00f) { --me.y; --pos.y; ++est.y; }
	else
	{
		me.y += est.y;
		pos.y += est.y;
		est.y = 0.00f;
	}

	struct args {
		float& PreY;
		float& posY;
		float& estY;
	};

	args ar = { preY,pos.y,est.y };
	auto par = &ar;

	auto hitAct = [](void* arg_) {
		auto a = reinterpret_cast<args*>(arg_);
		a->posY = a->PreY;
		a->estY = 0.00f;
	};

	CheckHitToGrounds(me, hitAct, par);


}
