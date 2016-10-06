
/*==============================================================================
                                 INCLUDE FILES
 =============================================================================*/

#include "tlv.h"
#include "net_tlv.h"




/*==============================================================================
                                 API FUNCTIONS
 =============================================================================*/

int8_t tlv_init(tlv_t ** tlv, uint8_t * ptr )
{
	if( ptr == NULL)
		return 0 ;


	*tlv= (tlv_t* )  ptr;
	return 1 ;
}


int8_t tlv_leader_init(tlv_leader_t **tlv, uint8_t * ptr )
{
	if( ptr == NULL)
		return 0 ;


	*tlv= (tlv_leader_t* )  ptr;
	return 1 ;
}

int8_t tlv_route64_init(tlv_route64_t **tlv, uint8_t * ptr )
{
	if( ptr == NULL)
		return 0 ;


	*tlv= (tlv_route64_t* )  ptr;
	return 1 ;
}

int8_t tlv_connectivity_init(tlv_connectivity_t **tlv, uint8_t * ptr )
{
	if( ptr == NULL)
		return 0 ;


	*tlv= (tlv_connectivity_t* )  ptr;
	return 1 ;

}

// LZ. -------------------------------------------------------------------------

int8_t tlv_target_eid_init(net_tlv_target_eid_t **tlv, uint8_t *ptr)
{
	if( ptr == NULL )
		return 0 ;

	*tlv= (net_tlv_target_eid_t*) ptr;
	return 1 ;
}

int8_t tlv_rloc16_init(net_tlv_rloc16_t **tlv, uint8_t *ptr)
{
	if( ptr == NULL )
		return 0 ;

	*tlv= (net_tlv_rloc16_t*) ptr;
	return 1 ;
}

int8_t tlv_ml_eid_init(net_tlv_ml_eid_t **tlv, uint8_t *ptr)
{
	if( ptr == NULL )
		return 0 ;

	*tlv= (net_tlv_ml_eid_t*) ptr;
	return 1 ;
}

// -----------------------------------------------------------------------------


int8_t tlv_write(tlv_t *tlv, tlv_type_t type, int8_t length, uint8_t * value )
{
	if(tlv == NULL )
		return 0;

	tlv->type=(uint8_t)type;
	tlv->length=length;
	memcpy(tlv->value, value, tlv->length);
	return 1 ;
}


tlv_t* tlv_find(uint8_t * buf, uint8_t buf_length, const tlv_type_t type)
{
	tlv_t * tlv;
	uint8_t i=0;

	tlv_init(&tlv,buf);
	while( tlv < (tlv_t*)&buf[buf_length])
	{
		if (tlv->type==type )
		{
			printf(" TLV exist ...\n" );
			return  tlv;
		}
		i+=(tlv->length+2);
		tlv_init(&tlv,&buf[i]);
	}
	return NULL;
}



int8_t tlv_print(tlv_t* tlv)
{
	PRINTF("  + type :");
	switch (tlv->type)
	{
	case TLV_SOURCE_ADDRESS:
		PRINTF(" SOURCE_ADDRESS    ");
		break;
	case TLV_MODE:
		PRINTF(" MODE              ");
		break;
	case TLV_TIME_OUT:
		PRINTF(" TIME_OUT 	     ");
		break;
	case TLV_CHALLENGE:
		PRINTF(" CHALLENGE         ");
		break;
	case TLV_RESPONSE:
		PRINTF(" RESPONSE          ");
		break;
	case TLV_LINK_LAYER_FRAME_COUNTER:
		printf(" LINK_LAYER_FRAME_COUNTER");
		break;
	case TLV_LINK_QUALITY:  // Not used in Thread Network
		PRINTF(" LINK_QUALITY      ");
		break;
	case TLV_NETWORK_PARAMETER:  // Not used in Thread Network
		PRINTF(" NETWORK_PARAMETER ");
		break;
	case TLV_MLE_FRAME_COUNTER:
		PRINTF(" MLE_FRAME_COUNTER ");
		break;
	case TLV_ROUTE64:
		PRINTF(" ROUTE64          ");
		break;
	case TLV_ADDRESS16:
		PRINTF(" ADDRESS16         ");
		break;
	case TLV_LEADER_DATA:
		PRINTF(" LEADER_DATA       ");
		break;
	case TLV_NETWORK_DATA:
		PRINTF(" NETWORK_DATA      ");
		break;
	case TLV_TLV_REQUEST:
		PRINTF(" TLV_REQUEST       ");
		break;
	case TLV_SCAN_MASK:
		PRINTF(" SCAN_MASK         ");
		break;
	case TLV_CONNECTIVITY:
		PRINTF(" CONNECTIVITY      ");
		break;
	case TLV_LINK_MARGIN:
		PRINTF(" LINK_MARGIN       ");
		break;
	case TLV_STATUS:
		PRINTF(" STATUS            ");
		break;
	case TLV_VERSION:
		PRINTF(" VERSION           ");
		break;
	case TLV_ADDRESS_REGISTRATION:
		PRINTF(" ADDRESS_REGISTRATION");
		break;
	default:
		PRINTF("Error tlv type not recognized ");
		return 0 ;
		break;
	}
	PRINTF(", length : %i , value :  ",tlv->length);

	for(uint8_t i=0 ; i<tlv->length; i++) {
		PRINTF("%02x ", tlv->value[i]);
	}
	PRINTF("\r\n");
	return 1;

}



