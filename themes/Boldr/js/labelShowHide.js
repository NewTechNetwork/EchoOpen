
//$('.formSubmit').each(function() {
//							   
//	var dataCheck = $ ('.inputData').attr('title');
//	if($('.inputData').val() == dataCheck){
//		$('.submitBtn').css('display','block');
//	}
//	else {
//		$('.submitBtn').css('display','none');
//
//     }
//});

$('.inputData').each(function() {
	var as = $(this).attr('title');
	$(this).val(as);
});

$('.inputData').focus(function() {
	$(this).val('');
});


$('.inputData').blur(function() {
	var as = $(this).attr('title');
	if($ (this).val() === '') {
	$(this).val(as);

//	var dataCheck = $ ('.inputData').attr('title');
//	if($('.inputData').val() == dataCheck){
//		$('.submitBtn').css('display','none');
//	}
//	else {
//		$('.submitBtn').css('display','block');
//
//     }

}

});

