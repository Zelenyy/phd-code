//
// Created by zelenyy on 16.07.2020.
//
#ifndef PHD_CODE_PARTICLEFIELD_HH
#define PHD_CODE_PARTICLEFIELD_HH

template<class R, class T>
class LateChangedField{
protected:
    T* field = nullptr;
    R back_field;
    virtual void update(){
        is_changed = false;
    };
    bool is_changed;
public:



    explicit LateChangedField(R init){
        set(init);
    }

    T* get(){
        if (is_changed){
            update();
        }
        return field;
    }

    R getBackField(){
        return back_field;
    }

    void set(R field){
        back_field = field;
        is_changed = true;
    }
};

class ParticleField : public LateChangedField<G4String, G4ParticleDefinition>{
public:
    explicit ParticleField(G4String name) : LateChangedField(std::move(name)){};
protected:
    void update() override{
        LateChangedField<G4String, G4ParticleDefinition>::update();
        auto pt = G4ParticleTable::GetParticleTable();
        field = pt->FindParticle(back_field);
    }
};

#endif //PHD_CODE_PARTICLEFIELD_HH
